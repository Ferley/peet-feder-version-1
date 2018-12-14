#include "load_cell.h"

HX711 scale;

//#define CELL_CALIBRATE  // uncomment if you want to calibrate the bowl 
//float pesoActual=0.0;

void HX711_calibration(void){
  //Serial.println("Initializing the scale");
  // parameter "gain" is ommited; the default value 128 is used by the library
  // HX711.DOUT  - pin #26
  // HX711.PD_SCK - pin #25
  scale.begin(26, 25);



  #ifdef CELL_CALIBRATE
  float gramosToma;
  scale. set_scale();
  scale.tare();
  Serial.println("Put known weight on ");
  //displayString("Calibrate", 64, 10);
  delay(2500);
  Serial.print(scale.get_units(10));
  Serial.print(" Divide this value to the weight and insert it in the scale.set_scale() statement");
  //displayFloat(scale.get_units(10), 64, 15);
  while(1==1);
    Serial.println("Before setting up the scale:");
    Serial.print("read: \t\t");
    Serial.println(scale.read());     // print a raw reading from the ADC

    Serial.print("read average: \t\t");
    Serial.println(scale.read_average(20));   // print the average of 20 readings from the ADC

    Serial.print("get value: \t\t");
    gramosToma =scale.get_value(10);
    Serial.println(gramosToma);    // print the average of 5 readings from the ADC minus the tare weight (not set yet)

    Serial.print("get units: \t\t");
    Serial.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
  #endif
  // by the SCALE parameter (not set yet)
   /**/
  scale.set_scale(1946.99661); 
  //scale.set_scale(-1941.94);    // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();                   // reset the scale to 0

  /**/
  #ifdef CELL_CALIBRATE
    Serial.println("After setting up the scale:");

    Serial.print("read: \t\t");
    Serial.println(scale.read());                 // print a raw reading from the ADC

    Serial.print("read average: \t\t");
    Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

    Serial.print("get value: \t\t");
    Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()

    Serial.print("get units: \t\t");
    Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
    // by the SCALE parameter set with set_scale

    Serial.println("Readings:");
  #endif
  /**/

}

float HX711_read(void){
  Serial.print("pesoActual: \t\t");
  return scale.get_units(5);    
  
}
