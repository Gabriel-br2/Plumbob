//-------------------------------------------------------------------------------------------------
// Redo the aligning of the vector

bool allzero(float vetor[]){
  for (int i = 0; i < n; i++) {
    if (vetor[i] != 0) {
      return true;
    }
  }
  return false;
}

//-------------------------------------------------------------------------------------------------
// Redo the aligning of the vector

void realignVec(float* vec){
  if (vec[0] != 0){
    return;
  }
 
  else{
    for(int i = 0;i < (n-1);i++){
      vec[i] = vec[i+1]; 
    }
    vec[n-1] = 0;
    realignVec(vec);
  }
}

//-------------------------------------------------------------------------------------------------
// Button check for reset

void checkButton(float time){
  if (digitalRead(BUTTON_PINr) == LOW && last_buttonR_state){
    last_buttonR_state = false;
    digitalWrite(LED_BUTTONr, HIGH);
       
    for(int j = 0; j < (n-1); j++){  
      if ((((millis()/1000) - timeVec[j]) >= time) && (timeVec[j] != 0)){
        timeVec[j] = 0;
        count_buttonR_pressed--;
      }
    }

    if (allzero(timeVec)){
      realignVec(timeVec);
    }

    timeVec[count_buttonR_pressed] = millis()/1000;  
    count_buttonR_pressed++;
    
    Serial.print("Button pressed | ");
    Serial.print("count:  ");
    Serial.print(count_buttonR_pressed);
    Serial.print("| Vec: [");

    Serial.print(timeVec[0]);
    for (int y = 1; y < n; y++){
      Serial.print(", ");
      Serial.print(timeVec[y]);
    }

    Serial.println("]");

    if (count_buttonR_pressed >= n){
      Serial.println("Reset to default config");
      count_buttonR_pressed = 0;
      for (int k = 0; k < n; k++){
        timeVec[k] = 0;
      }
      digitalWrite(LED_BUTTONr, LOW);
      wm.resetSettings();
      ESP.restart();      
    }
    delay(500);
  }

  else if (digitalRead(BUTTON_PINr) == HIGH){
    digitalWrite(LED_BUTTONr, LOW);
    last_buttonR_state = true;
  }
}

//-------------------------------------------------------------------------------------------------
// Hex to RGB

RGB hexToRGB(const String& hexCode){
  RGB rgb;
  String sanitizedHex = hexCode;  
  unsigned long hexValue = strtoul(sanitizedHex.c_str(), NULL, 16);
  
  rgb.red = (hexValue >> 16) & 0xFF;
  rgb.green = (hexValue >> 8) & 0xFF;
  rgb.blue = hexValue & 0xFF;
  
  return rgb;
}