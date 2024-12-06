#pragma once
#include <iostream>
struct bit_position{
    bit_position(){}
    bit_position(int y, int x){
        x_position = x;
        y_position = y;
    }
    bool  operator==(bit_position const& obj){
        return (x_position == obj.x_position && y_position == obj.y_position);
    }
    bit_position&  operator=(bit_position const& obj){
        x_position = obj.x_position;
        y_position = obj.y_position;
        return *this;
    }
    int y_position;
    int x_position;
    
};