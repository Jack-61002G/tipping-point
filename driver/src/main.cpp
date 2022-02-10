#include "main.h"
using namespace okapi;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol()
{
	// Chassis Controller - lets us drive the robot around with open- or closed-loop control
	std::shared_ptr<ChassisController> drive =
		ChassisControllerBuilder()
			.withMotors({-14, 10}, {-4, 20})
			// Green gearset, 4 in wheel diam, 11.5 in wheel track
			.withDimensions(AbstractMotor::gearset::green, {{4_in, 11.5_in}, imev5GreenTPR})
			.build();

	ControllerButton armUpButton(ControllerDigital::L1);
	ControllerButton armDownButton(ControllerDigital::L2);

	Motor armMotor(-8);
	armMotor.setBrakeMode(AbstractMotor::brakeMode::hold);

	ControllerButton clawup(ControllerDigital::R1);
	ControllerButton clawdown(ControllerDigital::R2);
	Motor clawMotor(9);
	clawMotor.setBrakeMode(AbstractMotor::brakeMode::hold);
	 

	ControllerButton clampup(ControllerDigital::right);
	ControllerButton clampdown(ControllerDigital::Y);
	Motor clampmotor(19);
	clampmotor.setBrakeMode(AbstractMotor::brakeMode::hold);

	// Joystick to read analog values for tank or arcade control
	// Master controller by default
	Controller controller;

	while (true)
	{

		// Arcade drive with the left stick.
		drive->getModel()->arcade(controller.getAnalog(ControllerAnalog::leftY),
								  controller.getAnalog(ControllerAnalog::leftX));

		if (armUpButton.isPressed()) {armMotor.moveVoltage(12000);} 
		else if (armDownButton.isPressed()) {armMotor.moveVoltage(-12000);} 
		else {armMotor.moveVoltage(0);}

		if (clawup.isPressed()) {clawMotor.moveVoltage(12000);} 
		else if (clawdown.isPressed()) {clawMotor.moveVoltage(-12000);} 
		else {clawMotor.moveVoltage(0);}
		
		if (clampup.isPressed()) {clampmotor.moveVoltage(12000);} 
		else if (clampdown.isPressed()) {clampmotor.moveVoltage(-12000);} 
		else {clampmotor.moveVoltage(0);}

		// Wait and give up the time we don't need to other tasks.
		// joystick values, motor telemetry, etc. all updates every 10 ms.
		pros::delay(10);
	}
}
