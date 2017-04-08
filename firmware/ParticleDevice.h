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
    
    ParticleDevice() : ParticleDevice(DeviceEnum::UNKNOWN) { }
    ParticleDevice( DeviceEnum d ) : device(d) { }
    
    DeviceEnum id(){
        if( DeviceEnum::UNKNOWN == this->device ){
            this->device = id_device();        
        }
        return this->device;
    }
    
    String name(){
        switch( this->device ){
            case DeviceEnum::ELI:
                return "Eli";
            case DeviceEnum::LOGAN:
                return "Logan";
            case DeviceEnum::MATT:
                return "Matt";
            case DeviceEnum::PARKER:
                return "Parker";
            case DeviceEnum::COMRAD:
                return "Comrad";
            default:
                return "Unknown";
        }
    }
    
    
    
    DeviceEnum device;
    
};

//iot
}