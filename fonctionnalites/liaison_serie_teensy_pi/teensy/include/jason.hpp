#ifndef JASON_HPP
#define JASON_HPP 

// C'est jason qui connait les structures qui lui sont envoyées, 
// donc c'est lui qui les expose aux autres pour leur en faire profiter.
struct GpsData {
  float lat;
  float lon;
};

struct RobotState {
  uint32_t timestamp;
  float battery_voltage;
  GpsData gps; // Structure imbriquée
};

struct Command {
  bool line;
  float angle;
  float x;
  float y;
};

extern GpsData data;
extern RobotState state;
extern Command commande;

void receive_jason_loop(void);
void send_jason_loop(void);

#endif