import json
from queue import Queue
import threading
import time
import datetime
import serial

# --- CONFIGURATION ---

# rosbag
ROSBAG_WANTED = True
filename = datetime.datetime.now().strftime("rosbag-%Y-%m-%d-%H-%M.json")

#communication série entre pi et teensy
ser = serial.Serial('/dev/ttyACM0', 500000, timeout=0.05)
""" le timeout de 50 ms veut dire que le ser.readline() va abandonner la lecture au bout de 50 ms s'il n'a rien reçu
ça nous fait une fréquence de 20Hz ce qui est largement suffisant pour le haut niveau. 
On ne va jamais envoyer 20 ordres de direction en 1 seconde 
vu qu'on attends que l'ordre se soit bien réalisé avant d'en envoyer un autre"""
structs = None

# File d'attente pour les commandes à envoyer
command_queue = Queue()

# --- CONFIGURATION DES FLAGS DE DEBUG ---
DEBUG_FLAGS = {
    "telemetry": True,      # Affiche la télémétrie du robot
    "serial_reader": True,  # Affiche les ordres reçus de la Teensy
    "serial_writer": True, # Affiche les ordres envoyés à la Teensy
    "errors": True,  # Affiche les erreurs critiques
}

def debug(flag_name, message):
  """Affiche le message si et seulement si le flag demandé est activé à True."""
  if DEBUG_FLAGS.get(flag_name, False):
    print(f"[DEBUG:{flag_name.upper()}] {message}")

def serial_reader_thread():
  """Thread d'arrière-plan dédié à l'écoute continue de la télémétrie de la Teensy"""
  debug("serial_reader",'Démarrage de l’écoute...')

  while True:
    line_bytes = ser.readline()

    if not line_bytes:
      continue # recommence la boucle, tant qu'il n'y a pas de lignes à lire

    # au cas où la teensy redémarre ou que le cable se débranche, on ignore les paquets corrompus par sécurité
    try:
      line = line_bytes.decode('utf-8').strip()
      data = json.loads(line)
    except (UnicodeDecodeError, json.JSONDecodeError):
      # Ignore un paquet corrompu isolé sans tuer le thread de lecture
      continue # vers la prochaine boucle du while True
    
    # Si on arrive ici, on est sûr qu'une ligne a été lue, et correctement   

    # Enregistrement dans le rosbag si souhaité
    if ROSBAG_WANTED:
      with open(filename,"a") as f:
        f.write(line + "\n")
    
    # et enfin, on renvoit data
    global structs #global sert à dire qu'on modifie la variable globale structs et qu'on est pas en train d'en créer une dans la boucle
    structs = data


def serial_writer_thread():
  """Thread d'arrière-plan ou gestionnaire d'envoi qui vide la file de commandes"""
  while True:
    # Récupère une commande dans la file (bloque tant qu'il n'y a rien à envoyer)
    cmd_dict = command_queue.get()
    if cmd_dict is None:
      break

    try:
      message = json.dumps(cmd_dict) + '\n'
      ser.write(message.encode('utf-8'))
      ser.flush()  # S'assure que c'est bien parti
      debug("serial_writer",f'Ordre envoyé : {cmd_dict}')
    except Exception as e:
      debug("serial_writer",f'{e}')

    command_queue.task_done()


# --- LANCEMENT DES THREADS ---
reader = threading.Thread(target=serial_reader_thread, daemon=True)
writer = threading.Thread(target=serial_writer_thread, daemon=True)

reader.start()
writer.start()

# --- BOUCLE DE CODE PRINCIPALE (ex: intelligence artificielle, navigation, IA, etc.) ---
try:
  # TODO : vérifier qu'on ne reçoit jamais une commande en angle et en distance en même temps. Sinon renvoyer une erreur
  # La Pi peut envoyer des ordres quand il veut, totalement indépendamment de la télémétrie.
  time.sleep(2)
  print("\n--- Simulation : Ordre de rotation de 90° ---")
  command_queue.put({'cmd': 'rotation', 'angle': 90.0})

  time.sleep(3)
  print('\n--- Simulation : Ordre de ligne droite vers X=2.0, Y=5.5 ---')
  command_queue.put({'cmd': 'ligne', 'dist': 2.0}) # positif = avancer, négatif = reculer. 

  # Exemple de récupération de données dans la struct pour les imprimer
  while True:
    
    lat=structs['gps']['lat']
    long=structs['gps']['lon']

    debug("telemetry",f"Télémétrie -> Lat: {lat}, Lon: {long}")
    time.sleep(0.2) # on ne les print que 5 fois par seconde sinon relou pour y voir quoi que ce soit

except KeyboardInterrupt:
  debug("errors","Arrêt demandé par l'utilisateur...")
  ser.close()
  debug("errors",'Fermeture propre.')
