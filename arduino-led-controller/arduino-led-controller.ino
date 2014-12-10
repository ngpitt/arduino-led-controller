// Inputs must be analog (AX) pins
#define COLOR_INPUT      A0
#define BRIGHTNESS_INPUT A1
#define STROBE_INPUT     A2

// Color outputs must be PWM (~) pins
#define RED_OUTPUT       9
#define GREEN_OUTPUT     10
#define BLUE_OUTPUT      11

// Strip outputs can be any pins
#define STRIP1_OUTPUT    0
#define STRIP2_OUTPUT    1
#define STRIP3_OUTPUT    2
#define STRIP4_OUTPUT    3
#define STRIP5_OUTPUT    4
#define STRIP6_OUTPUT    5
#define STRIP7_OUTPUT    6
#define STRIP8_OUTPUT    7
#define STRIP9_OUTPUT    8
#define STRIP10_OUTPUT   12
#define STRIP11_OUTPUT   13
#define STRIP12_OUTPUT   A3
#define STRIP13_OUTPUT   A4

// Number of samples to use
#define SAMPLES          100

// Define global variables
int ColorSamples[SAMPLES], BrightnessSamples[SAMPLES], StrobeSamples[SAMPLES], SampleIndex;
const double ColorDivisor = 1022 / 6.0; // 1023 - 1 (value reserved for white)
bool StrobeState = true;
unsigned long LastStrobe = 0;

// Average an array of samples
int averageSamples(int samples[])
{
  double sum = 0;
  
  for (int i = 0; i < SAMPLES; i++)
  {
    sum += samples[i];
  }
  
  return round(sum / SAMPLES);
}

// Update the color and brightness of all strips
void updateStrips()
{
  double red = 0, green = 0, blue = 0;
  
  // Calculate the average color
  int color = averageSamples(ColorSamples);
  
  // Use 1023 for white
  if (color == 1023) {
    red = 1;
    green = 1;
    blue = 1;
  }
  // Calculate RGB values
  else
  {
    // Calculate constants
    double selector = color / ColorDivisor;
    double i = 1 - abs(fmod(selector, 2) - 1);
    
    // Select which two colors to use
    if (selector >= 0 && selector < 1) {
        red = 1;
        green = i;
    }
    else if (selector >= 1 && selector < 2)
    {
        red = i;
        green = 1;
    }
    else if (selector >= 2 && selector < 3)
    {
        green = 1;
        blue = i;
    }
    else if (selector >= 3 && selector < 4)
    {
        green = i;
        blue = 1;
    }
    else if (selector >= 4 && selector < 5)
    {
        red = i;
        blue = 1;
    }
    else
    {
        red = 1;
        blue = i;
    }
  }
  
  // Calculate the average brightness
  double brightness = averageSamples(BrightnessSamples) / 1023.0;
  
  // Write the output to the respective pin
  analogWrite(RED_OUTPUT, round(floor(red * 255) * brightness));
  analogWrite(GREEN_OUTPUT, round(floor(green * 255) * brightness));
  analogWrite(BLUE_OUTPUT, round(floor(blue * 255) * brightness));
}

// Write to all strips
void setStrips(bool value)
{
  digitalWrite(STRIP1_OUTPUT, value);
  digitalWrite(STRIP2_OUTPUT, value);
  digitalWrite(STRIP3_OUTPUT, value);
  digitalWrite(STRIP4_OUTPUT, value);
  digitalWrite(STRIP5_OUTPUT, value);
  digitalWrite(STRIP6_OUTPUT, value);
  digitalWrite(STRIP7_OUTPUT, value);
  digitalWrite(STRIP8_OUTPUT, value);
  digitalWrite(STRIP9_OUTPUT, value);
  digitalWrite(STRIP10_OUTPUT, value);
  digitalWrite(STRIP11_OUTPUT, value);
  digitalWrite(STRIP12_OUTPUT, value);
  digitalWrite(STRIP13_OUTPUT, value);
}

void setup()
{
  // Initialize inputs
  pinMode(COLOR_INPUT, INPUT);
  pinMode(BRIGHTNESS_INPUT, INPUT);
  
  // Initialize color outputs
  pinMode(RED_OUTPUT, OUTPUT);
  pinMode(GREEN_OUTPUT, OUTPUT);
  pinMode(BLUE_OUTPUT, OUTPUT);
  
  // Initialize strip outputs
  pinMode(STRIP1_OUTPUT, OUTPUT);
  pinMode(STRIP2_OUTPUT, OUTPUT);
  pinMode(STRIP3_OUTPUT, OUTPUT);
  pinMode(STRIP4_OUTPUT, OUTPUT);
  pinMode(STRIP5_OUTPUT, OUTPUT);
  pinMode(STRIP6_OUTPUT, OUTPUT);
  pinMode(STRIP7_OUTPUT, OUTPUT);
  pinMode(STRIP8_OUTPUT, OUTPUT);
  pinMode(STRIP9_OUTPUT, OUTPUT);
  pinMode(STRIP10_OUTPUT, OUTPUT);
  pinMode(STRIP11_OUTPUT, OUTPUT);
  pinMode(STRIP12_OUTPUT, OUTPUT);
  pinMode(STRIP13_OUTPUT, OUTPUT);
  
  // Initialize sample arrays
  for (SampleIndex = 0; SampleIndex < SAMPLES; SampleIndex++)
  {
    ColorSamples[SampleIndex] = analogRead(COLOR_INPUT);
    BrightnessSamples[SampleIndex] = analogRead(BRIGHTNESS_INPUT);
    StrobeSamples[SampleIndex] = analogRead(STROBE_INPUT);
  }
  
  // Initialize the color and brightness of all strips
  updateStrips();
   
  // Execute boot sequence
  digitalWrite(STRIP1_OUTPUT, HIGH);
  delay(100);
  digitalWrite(STRIP2_OUTPUT, HIGH);
  delay(100);
  digitalWrite(STRIP3_OUTPUT, HIGH);
  delay(100);
  digitalWrite(STRIP4_OUTPUT, HIGH);
  delay(100);
  digitalWrite(STRIP5_OUTPUT, HIGH);
  delay(100);
  digitalWrite(STRIP6_OUTPUT, HIGH);
  delay(100);
  digitalWrite(STRIP7_OUTPUT, HIGH);
  delay(100);
  digitalWrite(STRIP8_OUTPUT, HIGH);
  delay(100);
  digitalWrite(STRIP9_OUTPUT, HIGH);
  delay(100);
  digitalWrite(STRIP10_OUTPUT, HIGH);
  delay(100);
  digitalWrite(STRIP11_OUTPUT, HIGH);
  delay(100);
  digitalWrite(STRIP12_OUTPUT, HIGH);
  delay(100);
  digitalWrite(STRIP13_OUTPUT, HIGH);
}

void loop()
{
  // Update the sample arrays
  if (SampleIndex >= SAMPLES)
  {
    SampleIndex = 0;
  }
  ColorSamples[SampleIndex] = analogRead(COLOR_INPUT);
  BrightnessSamples[SampleIndex] = analogRead(BRIGHTNESS_INPUT);
  StrobeSamples[SampleIndex] = analogRead(STROBE_INPUT);
  SampleIndex++;
  
  // Update the color and brightness of all strips
  updateStrips();
  
  // Calculate the average strobe 
  int strobe = averageSamples(StrobeSamples);
  
  // Strobe if above the threshold
  if (strobe >= 10)
  {
    // Strobe all strips
    if (millis() >= LastStrobe + strobe)
    {
      StrobeState = !StrobeState;
      setStrips(StrobeState);
      LastStrobe = millis();
    }
  }
  // Default to on
  else
  {
    StrobeState = true;
    setStrips(StrobeState);
  }
}
