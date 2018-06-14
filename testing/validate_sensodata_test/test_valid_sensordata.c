#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_TEMP 100
#define MIN_TEMP 0
#define MAX_LOUD 1024
#define MIN_LOUD 0
#define MAX_HUM 1024
#define MIN_HUM 0
#define MAX_RAD 1024
#define MIN_RAD 0

typedef struct msg_ {
  uint8_t flags;
  uint16_t sourceID;
  int32_t timestamp;
  uint8_t compression;
  float temp;
  float hum;
  float rad;
  float loud;
}msg;

typedef struct data_{
  uint32_t time;      //minutes since 1900-01-01
  uint8_t comp;       //amount of packets merged into one
  int16_t temp;        //temperature in deegrees celsius *10
  uint8_t hum;        //humidity in percent
  uint16_t rad;       //visible light in lumen
  uint8_t loud;       //loudness in decibel
}data;

typedef struct plant_info_{
  uint8_t flags;
  float temp_opt;
  float temp_weight;
  float hum_opt;
  float hum_weight;
  float rad_opt;
  float rad_weight;
  float loud_opt;
  float loud_weight;
}plant_info;

//global testdata
float glob_hum;
float glob_temp;
float glob_rad;
float glob_loud;

float timestamp() {
  return 0;
}

float humidity() {
  return glob_hum;
}

float temperature() {
  return glob_temp;
}

float radiation(){
  return glob_rad;
}

float loudness() {
  return glob_loud;
}

void fill_in_sensor_data(data* new_data) {
  // fill struct fields
  new_data->time = timestamp();
  new_data->comp = 1;
  //new_data->temp = temperature() * 10; //save a decimal place by multiplying by 10
  new_data->temp = temperature();
  new_data->hum = humidity();
  new_data->rad = radiation();
  new_data->loud = loudness();
}

uint8_t is_valid_temp(float temperature){
  if(temperature > MAX_TEMP || temperature < MIN_TEMP) return 0;
  else return 1;
}

uint8_t is_valid_hum(float humidity){
  if(humidity > MAX_HUM || humidity < MIN_HUM) return 0;
  else return 1;
}

uint8_t is_valid_rad(float radiation){
  if(radiation > MAX_RAD || radiation < MIN_RAD) return 0;
  else return 1;
}

uint8_t is_valid_loud(float loudness){
  if(loudness > MAX_LOUD || loudness < MIN_LOUD) return 0;
  else return 1;
}

void set_glob_vars(float temp, float rad, float hum, float loud){
  glob_hum = hum;
  glob_temp = temp;
  glob_loud = loud;
  glob_rad = rad;
}

msg* packMsg(data* payload){
  msg* msg_buffer = (msg*) malloc(sizeof(msg));
  msg_buffer->timestamp = payload->time;
  msg_buffer->compression = payload->comp;
  if(is_valid_temp(payload->temp))msg_buffer->temp = payload->temp;
  if(is_valid_hum(payload->hum))msg_buffer->hum = payload->hum;
  if(is_valid_rad(payload->rad))msg_buffer->rad = payload->rad;
  if(is_valid_temp(payload->loud))msg_buffer->loud = payload->loud;
  return msg_buffer;
}

int main(){
  //normal case
  set_glob_vars(20, 20, 20, 20);
  data* new_data = (data*) calloc(sizeof(data), 1);
  fill_in_sensor_data(new_data);
  msg* msg = packMsg(new_data);
  printf("Normal Case: \ntemperature: %f \nradiation: %f \nhumidity: %f \nloudness: %f\n", msg->temp, msg->rad, msg->hum, msg->loud);
  set_glob_vars(120, 20,20,20);
  fill_in_sensor_data(new_data);
  msg = packMsg(new_data);
  printf("Invalid Temperature: \ntemperature: %f \nradiation: %f \nhumidity: %f \nloudness: %f\n", msg->temp, msg->rad, msg->hum, msg->loud);
  return 0;
}

