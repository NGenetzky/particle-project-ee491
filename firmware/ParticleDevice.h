#pragma once
#include "application.h"

// Improvement from "device.h" from old code.

namespace iot {
    
enum class DeviceEnum {
    COMRAD=0, MATT=1, PARKER=2, LOGAN=3, ELI=4,
    UNKNOWN
};

struct ParticleDevice {
    static String get_device_ID(){ return System.deviceID(); }
    
    static DeviceEnum id_device(){
        char first_unique = System.deviceID().charAt(0);
        switch(first_unique){
            case '2':
                return DeviceEnum::ELI;
            case '3':
                if(System.deviceID().charAt(1) == 9){
                    return DeviceEnum::LOGAN;
                } else {
                    return DeviceEnum::MATT;
                }
            case '4':
                return DeviceEnum::PARKER;
            case '5':
                return DeviceEnum::COMRAD;
            default:
                return DeviceEnum::UNKNOWN;
        }
    }
    
    ParticleDevice(){
        this->device = id_device();        
    }
    ParticleDevice( DeviceEnum d ) : device(d)
    { }
    
    String name(){
        switch( this->device ){
            case DeviceEnum::ELI:
                return "ELI";
            case DeviceEnum::LOGAN:
                return "LOGAN";
            case DeviceEnum::MATT:
                return "MATT";
            case DeviceEnum::PARKER:
                return "PARKER";
            case DeviceEnum::COMRAD:
                return "COMRAD";
            default:
                return "UNKNOWN";
        }
    }
    
    DeviceEnum device;
    
};

//iot
}