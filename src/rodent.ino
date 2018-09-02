#include <Arduino.h>
#include "StepperMotor.h"
#include "Thread.h"
#include "ThreadManager.h"
#include "Rodent.h"
#include "GCodeParser.h"
#include "GCode.h"
#include "RGBLeds.h"
#include "Switch.h"
#include "Speaker.h"
#include "Sound.h"
#include "EEPROMWriter.h"
#include "RotarySwitch.h"
#include "ManualController.h"
#include "RodentStatus.h"
#include "MemoryFree.h"
#include "EEPROM.h"

// constants
#define MAX_SERIAL_LINE_LEN 100
#define MAXIMUM_FEED_RATE 100
#define LINEAR_PITCH 0.8
#define ENCODER_PULSES_PER_REVOLUTION 1565

// motor constants
#define STEPS_PER_REVOLUTION 200

#define X_MOTOR_ENABLE 32
#define X_MOTOR_C1 33
#define X_MOTOR_C2 34

#define Y_MOTOR_ENABLE 29
#define Y_MOTOR_C1 30
#define Y_MOTOR_C2 31

#define Z_MOTOR_ENABLE 26
#define Z_MOTOR_C1 27
#define Z_MOTOR_C2 28

#define SPINDLE_PWM 4

#define BUZZER_PIN 39
#define RED_PIN 6
#define GREEN_PIN 7
#define BLUE_PIN 8

// limits
#define LIMIT_X 35
#define LIMIT_Y 36
#define LIMIT_Z 37

#define LEFT_ROTARY_SWITCH_PIN A0
#define RIGHT_ROTARY_SWITCH_PIN A1
#define ENCODER_PIN_A 21
#define ENCODER_PIN_B 20

#define AUTO_MANUAL_PIN 38
#define LID_LIMIT_PIN 40
#define EMERGENCY_PIN 41

#define SOFTWARE_SERIAL_RX 10
#define SOFTWARE_SERIAL_TX 11

#define MIN_DELAY_BETWEEN_STEPS 1500 // microseconds

// few important global vars
Rodent *rodent;
GCodeParser *parser = new GCodeParser();
ManualController *controller = new ManualController();
volatile int encoder_stack = 0;
Thread *manualMover = new Thread();

void setup() {
	rodent = new Rodent();
	Serial1.begin(9600);
	// init serial
	Serial.begin(9600);
	Serial2.begin(9600);
	Serial3.begin(9600);

	Serial1.setTimeout(50);
	Serial2.setTimeout(50); // time to buffer the incoming message
	Serial3.setTimeout(50); // time to buffer the incoming message


	// init stepper motors
	StepperMotor *stepperX = new StepperMotor(X_MOTOR_C1, X_MOTOR_C2, X_MOTOR_ENABLE, STEPS_PER_REVOLUTION);
	StepperMotor *stepperY = new StepperMotor(Y_MOTOR_C1, Y_MOTOR_C2, Y_MOTOR_ENABLE, STEPS_PER_REVOLUTION);
	StepperMotor *stepperZ = new StepperMotor(Z_MOTOR_C1, Z_MOTOR_C2, Z_MOTOR_ENABLE, STEPS_PER_REVOLUTION);

	// init stepper manager and attach the motors
	StepperManager *stepperManager = new StepperManager();
	stepperManager->setX(stepperX);
	stepperManager->setY(stepperY);
	stepperManager->setZ(stepperZ);
	stepperManager->setMinimumDelayBetweenSteps(MIN_DELAY_BETWEEN_STEPS);
	rodent->attachStepperManager(stepperManager);
	rodent->setPitches(LINEAR_PITCH, LINEAR_PITCH, LINEAR_PITCH);

	// set limit switches
	rodent->setLimits(LIMIT_X, LIMIT_Y, LIMIT_Z);

	// init rgb leds inside the Rodent's case
	RGBLeds *lights = new RGBLeds();
	lights->setRedPin(RED_PIN);
	lights->setGreenPin(GREEN_PIN);
	lights->setBluePin(BLUE_PIN);
	rodent->setRGBLightSystem(lights);

	// init switches
	Switch *manualSwitch = new Switch(AUTO_MANUAL_PIN);
	rodent->setManualSwitch(manualSwitch);

	Switch *lidLimit = new Switch(LID_LIMIT_PIN);
	rodent->setLidSwitch(lidLimit);

	Switch *emergencyButton = new Switch(EMERGENCY_PIN);
	rodent->setEmergencyButton(emergencyButton);

	// init rotary switches and rotary encoder of the manual controller
	RotarySwitch *leftSwitch = new RotarySwitch(A0);
	controller->setLeftSwitch(leftSwitch);

	RotarySwitch *rightSwitch = new RotarySwitch(A1);
	controller->setRightSwitch(rightSwitch);

	RotaryEncoder *encoder = new RotaryEncoder(ENCODER_PIN_A, ENCODER_PIN_B);
	controller->setRotaryEncoder(encoder);
	attachInterrupt(2, registerRotation, CHANGE);
	attachInterrupt(3, registerRotation, CHANGE);

	// inits the speaker
	Speaker *speaker = new Speaker(BUZZER_PIN);
	rodent->setSpeaker(speaker);

	// init threads and manager
	ThreadManager *threadManager = new ThreadManager();
	rodent->attachThreadManager(threadManager);

	Thread *stepperTimeouter = new Thread();
	stepperTimeouter->setInterval(100);
	stepperTimeouter->setFunction(steppersTimeout);
	stepperTimeouter->enable();
	threadManager->add(stepperTimeouter);

	// reads the main Serial
	Thread *serialReader = new Thread();
	serialReader->setInterval(5);
	serialReader->setFunction(serialFunc);
	serialReader->enable();
	threadManager->add(serialReader);

	// reads Serial2
	Thread *serial2Reader = new Thread();
	serial2Reader->setInterval(5);
	serial2Reader->setFunction(serial2Func);
	serial2Reader->enable();
	threadManager->add(serial2Reader);

	// reads Serial3
	Thread *Serial3Reader = new Thread();
	Serial3Reader->setInterval(5);
	Serial3Reader->setFunction(Serial3Func);
	Serial3Reader->enable();
	threadManager->add(Serial3Reader);

	// reads the software serial (esp8266)
	Thread *serial1Reader = new Thread();
	serial1Reader->setInterval(5);
	serial1Reader->setFunction(Serial1Func);
	serial1Reader->enable();
	threadManager->add(serial1Reader);

	// reports current values
	Thread *workCoordinateReporter = new Thread();
	workCoordinateReporter->setInterval(200);
	workCoordinateReporter->setFunction(reportWorkCoordinates);
	workCoordinateReporter->enable();
	threadManager->add(workCoordinateReporter);

	// reports current values
	Thread *targetCoordinateReporter = new Thread();
	targetCoordinateReporter->setInterval(200);
	targetCoordinateReporter->setFunction(reportTargetCoordinates);
	targetCoordinateReporter->enable();
	threadManager->add(targetCoordinateReporter);

	// reports target values
	Thread *machineCoordinateReporter = new Thread();
	machineCoordinateReporter->setInterval(200);
	machineCoordinateReporter->setFunction(reportMachineCoordinates);
	machineCoordinateReporter->enable();
	threadManager->add(machineCoordinateReporter);

	// checks the limit switches
	Thread *limitChecker = new Thread();
	limitChecker->setInterval(50);
	limitChecker->setFunction(limitCheck);
	limitChecker->enable();
	threadManager->add(limitChecker);

	// sends the "I WANT TO CONNECT"-message to the serial port
	Thread *connectionEstablisher = new Thread();
	connectionEstablisher->setInterval(50);
	connectionEstablisher->setFunction(tryConnect);
	connectionEstablisher->enable();
	threadManager->add(connectionEstablisher);

	// checks the manual switch
	Thread *manualSwitchChecker = new Thread();
	manualSwitchChecker->setInterval(50);
	manualSwitchChecker->setFunction(checkManualSwitch);
	manualSwitchChecker->enable();
	threadManager->add(manualSwitchChecker);

	// mvoes the Rodent's mill head if necessary (only in the manual mode)
	manualMover->setInterval(50);
	manualMover->setFunction(manualMove);
	manualMover->enable();
	threadManager->add(manualMover);

	// checks the lid's limit switch
	Thread *lidSwitchChecker = new Thread();
	lidSwitchChecker->setInterval(50);
	lidSwitchChecker->setFunction(checkLidSwitch);
	lidSwitchChecker->enable();
	threadManager->add(lidSwitchChecker);

	// checks the emergency button
	Thread *emergencyChecker = new Thread();
	emergencyChecker->setInterval(20);
	emergencyChecker->setFunction(checkEmergencyButton);
	emergencyChecker->enable();
	threadManager->add(emergencyChecker);

	// runs the speaker (changes or removes the tune if necessary)
	Thread *speakerThread = new Thread();
	speakerThread->setInterval(10);
	speakerThread->setFunction(runSpeaker);
	speakerThread->enable();
	threadManager->add(speakerThread);

	// checks the rotary switches of the manual controller
	Thread *switchCheck = new Thread();
	switchCheck->setInterval(50);
	switchCheck->setFunction(rotarySwitchCheck);
	switchCheck->enable();
	threadManager->add(switchCheck);

	// prints the available memory in to the main serial port
	Thread *memoryPrinter = new Thread();
	memoryPrinter->setInterval(500);
	memoryPrinter->setFunction(printMemory);
	memoryPrinter->enable();
	threadManager->add(memoryPrinter);

	// init spindle
	Spindle *spindle = new Spindle();
	spindle->setPWMPin(SPINDLE_PWM);
	rodent->attachSpindle(spindle);

	// init parser
	parser->setMaximumInputLength(MAX_SERIAL_LINE_LEN);
	stepperManager->setDelayBetweenSteps(1500);

	// Loads the StatusObject object from the EEPROM memory of the microcontroller.
	// Sets the Rodent's values to those which were saved to the EEPROM last time.
	StatusObject object = EEPROMWriter::read();
	rodent->setStatus(object);
	rodent->getRealCoordinateSystem()->setCurrentX(rodent->getMachineCoordinateSystem()->getCurrentX());
	rodent->getRealCoordinateSystem()->setCurrentY(rodent->getMachineCoordinateSystem()->getCurrentY());
	rodent->getRealCoordinateSystem()->setCurrentZ(rodent->getMachineCoordinateSystem()->getCurrentZ());
	rodent->getRealCoordinateSystem()->makeAllEqual();
}

// the main program
void loop() {
	rodent->getThreadManager()->runThreads();
}

// checks serial (main serial port (connected with the usb cable))
void serialFunc() {
	if (Serial.available() > 0) {
		char input[MAX_SERIAL_LINE_LEN];
		int i = Serial.readBytesUntil('\n', input, MAX_SERIAL_LINE_LEN);
		for (; i < MAX_SERIAL_LINE_LEN; i++) {
			input[i] = '\0';
		}

		//Serial.println("R19 " + String(input));

		GCode *code = parser->parse(input);
		// empty buffer
		for (int i = 0; i < MAX_SERIAL_LINE_LEN; i++) {
			input[i] = '\0';
			Serial.read();
		}

		rodent->obey(code); // do things with rodent
		// remove gcode pointer
		delete(code);
		code = NULL;
	}
}

// the X axis serial connection
// Used to ask the REAL coordinate values from the Arduino nano connected
// to the X axis' rotary encoder.
void serial2Func() {
	if (Serial2.available() > 0) {
		char input[MAX_SERIAL_LINE_LEN];
		int i = Serial2.readBytesUntil('\n', input, MAX_SERIAL_LINE_LEN);
		for (; i < MAX_SERIAL_LINE_LEN; i++) {
			input[i] = '\0';
		}

		GCode *code = parser->parse(input);
		long diff = (long)code->getR();

		if (diff != 0) {
			double increment = -diff / (ENCODER_PULSES_PER_REVOLUTION / LINEAR_PITCH);
			rodent->getRealCoordinateSystem()->setCurrentX(rodent->getRealCoordinateSystem()->getCurrentX() + increment);
		}

		// print the values to the main serial port
		Serial.print("R250 X");
		Serial.println(rodent->getRealCoordinateSystem()->getCurrentX());

		// turn the got flag up
		rodent->finallyGotRealX();

		// empty buffer and delete pointers
		for (int i = 0; i < MAX_SERIAL_LINE_LEN; i++) {
			input[i] = '\0';
			Serial2.read();
		}
		delete(code);
		code = NULL;
	}
}

// the Y axis serial connection
// Used to ask values from the Arduino Micro connected to the
// Y axis' rotary encoder.
void Serial3Func() {
	if (Serial3.available() > 0) {
		char input[MAX_SERIAL_LINE_LEN];
		int i = Serial3.readBytesUntil('\n', input, MAX_SERIAL_LINE_LEN);
		for (; i < MAX_SERIAL_LINE_LEN; i++) {
			input[i] = '\0';
		}

		GCode *code = parser->parse(input);
		long diff = (long)code->getR();

		if (diff != 0) {
			double increment = diff / (ENCODER_PULSES_PER_REVOLUTION / LINEAR_PITCH);
			rodent->getRealCoordinateSystem()->setCurrentY(rodent->getRealCoordinateSystem()->getCurrentY() + increment);
		}
	
		// print values to the main serial port
		Serial.print("R250 Y");
		Serial.println(rodent->getRealCoordinateSystem()->getCurrentY());

		// turn got flag up
		rodent->finallyGotRealY();

		// empty buffer and remove pointers
		for (int i = 0; i < MAX_SERIAL_LINE_LEN; i++) {
			input[i] = '\0';
			Serial3.read();
		}
		delete(code);
		code = NULL;
	}
}

void Serial1Func() {
	if (Serial1.available()) {
		char input[MAX_SERIAL_LINE_LEN];
		int i = Serial1.readBytesUntil('\n', input, MAX_SERIAL_LINE_LEN);
		for (; i < MAX_SERIAL_LINE_LEN; i++) {
			input[i] = '\0';
		}

		Serial.println(input);
		GCode *code = parser->parse(input);
		// empty buffer
		for (int i = 0; i < MAX_SERIAL_LINE_LEN; i++) {
			input[i] = '\0';
			Serial1.read();
		}

		rodent->obey(code); // do things with rodent
							// remove gcode pointer
		delete(code);
		code = NULL;
	}
}

// Sends current coordinate system's target coordinate values as a string
// to the main serial port.
void reportTargetCoordinates() {
	double x = rodent->getCurrentCoordinateSystem()->getTargetX();
	double y = rodent->getCurrentCoordinateSystem()->getTargetY();
	double z = rodent->getCurrentCoordinateSystem()->getTargetZ();
	String x_val = String(x);
	String y_val = String(y);
	String z_val = String(z);
	String vals = String("R28 X" + x_val + " Y" + y_val + " Z" + z_val);
	Serial.println(vals);
}

// Sends work coordinate system's current values as a string to the main serial port.
void reportWorkCoordinates() {
	if (rodent->isConnected()) {
		double x = rodent->getWorkCoordinateSystem()->getCurrentX();
		double y = rodent->getWorkCoordinateSystem()->getCurrentY();
		double z = rodent->getWorkCoordinateSystem()->getCurrentZ();
		String x_val = String(x);
		String y_val = String(y);
		String z_val = String(z);
		String vals = String("R3 X" + x_val + " Y" + y_val + " Z" + z_val);
		Serial.println(vals);
	}

}

// Sends machine coordinate system's curent values as a string to the main serial port.
void reportMachineCoordinates() {
	if (rodent->isConnected()) {
		double x = rodent->getMachineCoordinateSystem()->getCurrentX();
		double y = rodent->getMachineCoordinateSystem()->getCurrentY();
		double z = rodent->getMachineCoordinateSystem()->getCurrentZ();
		String x_val = String(x);
		String y_val = String(y);
		String z_val = String(z);
		String vals = String("R4 X" + x_val + " Y" + y_val + " Z" + z_val);
		Serial.println(vals);
		Serial1.println(vals);
	}
}

// Used to disable all stepper motors after the timeout time has gone.
void steppersTimeout() {
	if (rodent->getStepperManager()->isOnTimeout() && rodent->getStepperManager()->isTimedOut(millis())) {
		rodent->getStepperManager()->disableAll();
	}
}

// Reads all the limit switches connected to the Rodent machine and send the inverted
// values to the Rodent object for doing some cool things.
void limitCheck() {
	rodent->xIsLimiting(!digitalRead(LIMIT_X));
	rodent->yIsLimiting(!digitalRead(LIMIT_Y));
	rodent->zIsLimiting(!digitalRead(LIMIT_Z));
}

// Reads the manual switch connected to the front panel of the Rodent case.
void checkManualSwitch() {
	rodent->setManualState(rodent->getManualSwitch()->isOn());
}

// Reads the limit switch connected to the Rodent's case's lid.
void checkLidSwitch() {
	rodent->setLidState(!rodent->getLidSwitch()->isOn());
}

// Reads the emergency button's digital value and delivers it to the Rodent object.
void checkEmergencyButton() {
	rodent->setEmergencyButtonState(rodent->getEmergencyButton()->isOn());
}

// Runs the speaker. If it's time to change the tune of the speaker, this will do it.
void runSpeaker() {
	if (rodent->getSpeaker()->isTimeToChange()) {
		rodent->getSpeaker()->goOn();
	}
}

// Tries to connect to the Rodentia controlling software. Sends a specific
// string to the serial port very fast until the right message comes back.
void tryConnect() {
	if (!rodent->isConnected()) {
		rodent->sayHelloToSerial();
	}
}

// Reads the digital values from the Rodent's manual controller's both rotary switches.
void rotarySwitchCheck() {
	controller->getLeftSwitch()->checkInputValue();
	controller->getRightSwitch()->checkInputValue();
}

// Interrupt function used to calculate Rodent's manual controller's rotary encoder's values.
void registerRotation() {
	if (rodent->isInManualMode()) {
		controller->getRotaryEncoder()->registerRotation();
	}
}

// Function for manual moving if it's necessary.
void manualMove() {
	if (!rodent->isInManualMode()) {
		return;
	}
	CoordinateSystem *cs = rodent->getCurrentCoordinateSystem();
	long diff = controller->getRotaryEncoder()->read();
	double diffInX = cs->getTargetX() - cs->getCurrentX();
	double diffInY = cs->getTargetY() - cs->getCurrentY();
	double diffInZ = cs->getTargetZ() - cs->getCurrentZ();
	if (diff == 0 && diffInX == 0 && diffInY == 0 && diffInZ == 0) {
		return;
	}

	int axis = controller->getLeftSwitch()->getCurrentPosition();
	int factor = controller->getRightSwitch()->getCurrentPosition();
	
	double increment = diff * (0.01 + 0.01 * factor);
	switch (axis) {
	case 0: // X axis
		rodent->setTargetX(cs->getTargetX() + increment);
		break;
	case 1: // Y axis
		rodent->setTargetY(cs->getTargetY() + increment);
		break;
	case 2: // Z axis
		rodent->setTargetZ(cs->getTargetZ() + increment);
		break;
	default:
		break;
	}

	if (!rodent->isMoving()) {
		rodent->getStepperManager()->enableAll();
		int mov = rodent->move();
		mov == 0 ? rodent->isMoving(false) : void();
	}

}

// Sends available memory percentage to the Rodent's main serial port.
void printMemory() {
	Serial.print("R200 M");
	Serial.println((int)(freeMemory() / 80));
}
