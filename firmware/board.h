
#ifndef CMD_PARTICLE_H
#define CMD_PARTICLE_H

int led_on();
int led_off();
int reset_board();
void disable_cloud_ifD6();

//Electron does not have wifi.
// #if ((PLATFORM_ID == 0) || (PLATFORM_ID == 6)) //Core or Photon only.
void disable_wifi_ifD6();

#endif // CMD_PARTICLE_H
