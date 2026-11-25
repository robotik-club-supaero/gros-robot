# type: ignore

import math
import threading
import time

import board
import digitalio
from adafruit_rgb_display import st7789
from PIL import Image, ImageDraw, ImageFont

# --- 1. CONFIGURATION MATÉRIELLE ---
cs_pin = digitalio.DigitalInOut(board.D17)
dc_pin = digitalio.DigitalInOut(board.D25)
reset_pin = digitalio.DigitalInOut(board.D24)
BAUDRATE = 24000000

spi = board.SPI()
disp = st7789.ST7789(
    spi,
    rotation=90,
    width=240,
    height=320,
    cs=cs_pin,
    dc=dc_pin,
    rst=reset_pin,
    baudrate=BAUDRATE,
)

# Dimensions globales
WIDTH = 320
HEIGHT = 240


def dessiner_asservissement(draw, font, temps_ecoule):
    """Page 1 : La courbe animée, confinée à l'espace de droite"""
    draw.text((10, 10), "Mode Asservissement",
              font=font, fill=(255, 255, 0))

    # Axe central de la zone de contenu
    draw.line([(0, HEIGHT//2), (WIDTH, HEIGHT//2)],
              fill=(50, 50, 50), width=1)

    points_courbe = []
    amplitude, amortissement, frequence = 80, 0.015, 0.1
    vitesse = 100
    decalage = temps_ecoule * vitesse

    for x in range(WIDTH):
        t = x - decalage
        y = amplitude * math.exp(-amortissement * x) * \
            math.sin(frequence * t)
        y_ecran = (HEIGHT // 2) - y
        points_courbe.append((x, int(y_ecran)))

    draw.line(points_courbe, fill=(0, 255, 0), width=2)


def main():
    image = Image.new("RGB", (WIDTH, HEIGHT))
    draw = ImageDraw.Draw(image)

    # Utilisation d'une police basique (à remplacer par truetype si besoin)
    font = ImageFont.load_default()

    # 1. Effacer l'écran (Fond noir pour la zone de droite)
    draw.rectangle((0, 0, WIDTH, HEIGHT),
                   outline=0, fill=(0, 0, 0))

    dessiner_asservissement(draw, font, temps_ecoule)

    # 4. Inverser l'image (si ton ST7789 fait effet miroir, sinon retire le transpose)
    image_a_afficher = image.transpose(Image.FLIP_LEFT_RIGHT)

    # 5. Envoyer à l'écran
    disp.image(image_a_afficher)

    # Petite pause pour ne pas saturer le CPU à 100%
    time.sleep(0.02)


if __name__ == "__main__":
    main()
