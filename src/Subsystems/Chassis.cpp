// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.




#include "Chassis.h"
#include "../RobotMap.h"
// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
#include "../Commands/DriveCmd.h"
// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

Chassis::Chassis() : Subsystem("Chassis") {
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
    leftMotor1 = RobotMap::chassisLeftMotor1;
    leftMotor2 = RobotMap::chassisLeftMotor2;
    rightMotor1 = RobotMap::chassisRightMotor1;
    rightMotor2 = RobotMap::chassisRightMotor2;
    driveMotors = RobotMap::chassisDriveMotors;
    compressor = RobotMap::chassisCompressor;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
    climbLS = RobotMap::chassisClimbLS;
    MotorDirect = 1;
    // Do not need to reverse motors with the Rocketbox PTO
    // driveMotors->SetInvertedMotor(RobotDrive::MotorType::kFrontLeftMotor,false);
    // driveMotors->SetInvertedMotor(RobotDrive::MotorType::kFrontRightMotor,false);
    // driveMotors->SetInvertedMotor(RobotDrive::MotorType::kRearLeftMotor,false);
    // driveMotors->SetInvertedMotor(RobotDrive::MotorType::kRearRightMotor,false);

    // Set control mode to direct throttle control, range -1 to 1, and set initial speed to zero
    // per section 3.3.2 C++ in TALON SRX Software Reference Manual (1/31/2016)
    /* leftMotor1->SetControlMode(CANSpeedController::kPercentVbus);
    leftMotor1->Set(0);
    leftMotor2->SetControlMode(CANSpeedController::kPercentVbus);
    leftMotor2->Set(0);
    rightMotor1->SetControlMode(CANSpeedController::kPercentVbus);
    rightMotor1->Set(0);
    rightMotor2->SetControlMode(CANSpeedController::kPercentVbus);
    rightMotor2->Set(0); */

    accel = new BuiltInAccelerometer;
}

void Chassis::InitDefaultCommand() {
    // Set the default command for a subsystem here.
    // SetDefaultCommand(new MySpecialCommand());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND

        SetDefaultCommand(new DriveCmd());

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND
}


// Put methods for controlling this subsystem
// here. Call these from Commands.

double Chassis::deadband(double JoystickValue,double DeadbandCutOff) {
	if (JoystickValue < DeadbandCutOff && JoystickValue > DeadbandCutOff*(-1)) {
		return 0;
	} else {
		return (JoystickValue - (abs(JoystickValue)/JoystickValue*DeadbandCutOff))/(1-DeadbandCutOff);
	}
}

void Chassis::DriveWithJoysticks(){
	float left = deadband(Robot::oi->getleftJoy()->GetY(),0.05);

	if (Robot::pneumaticSub->GetGShiftSolenoid()->Get()){
		printf("Climbing some cool stuff\n");
		float right = deadband(-1*Robot::oi->getGamepad()->GetThrottle(),0.05);
		leftMotor1->Set(left);
		leftMotor2->Set(left);
		if (right>0) {
			if (climbLS->Get()) {
				rightMotor1->Set(right);
				rightMotor2->Set(right);
			} else {
				rightMotor1->Set(0);
				rightMotor2->Set(0);
			}
		} else {
			rightMotor1->Set(right);
			rightMotor2->Set(right);
		}
	} else {
		printf("Driving some cool stuff");
		float right = deadband(Robot::oi->getrightJoy()->GetY(),0.05);
		if (Robot::oi->getrightJoy()->GetRawButton(1) && Robot::oi->getleftJoy()->GetRawButton(1)){
			left  = left * -1;
			right = right * -1;
			driveMotors->TankDrive(right,left);
		} else {
			driveMotors->TankDrive(left,right);
		}
	}
}

void Chassis::Stop(){
	driveMotors->StopMotor();
}

void Chassis::Compress() {
	compressor->SetClosedLoopControl(true);
	compressor->Start();
}

void Chassis::driveFeet(double feet=10,double speed=.5){
	// convert to feet
	feet = feet * 12;

	double distance=abs(feet/(3.1415 * diameter)*1000.0);
	while (leftMotor2->GetEncPosition()<distance) {
		leftMotor1->Set(-1 * speed);
		leftMotor2->Set(-1 *speed);
		rightMotor1->Set(speed);
		rightMotor2->Set(speed);
	}
	leftMotor1->Set(0);
	leftMotor2->Set(0);
	rightMotor1->Set(0);
	rightMotor2->Set(0);

}

void Chassis::PrintValues() {
	double X = accel->GetX();
	double Y = accel->GetY();
	double Z = accel->GetZ();

	// printf("X = %f, Y = %f, Z = %f\n",X,Y,Z);
	//SmartDashboard::PutString("DB/String 0","Accel: X");
	SmartDashboard::PutNumber("AccelX",X);
	//SmartDashboard::PutString("DB/String 1","Accel: Y");
	SmartDashboard::PutNumber("AccelY",Y);
	//SmartDashboard::PutString("DB/String 2","Accel: Z");
	SmartDashboard::PutNumber("AccelZ",Z);

	// Print encoder positions
	SmartDashboard::PutNumber("leftMotor2 Position: ",leftMotor2->GetEncPosition());
	SmartDashboard::PutNumber("leftMotor2 Velocity",leftMotor2->GetEncVel());
	SmartDashboard::PutNumber("rightMotor2 Position: ",rightMotor2->GetEncPosition());
	SmartDashboard::PutNumber("rightMotor2 Velocity",rightMotor2->GetEncVel());

}

void Chassis::resetPosition(){
    leftMotor1->SetPosition(0);
    leftMotor2->SetPosition(0);
    rightMotor1->SetPosition(0);
    rightMotor2->SetPosition(0);
}

void Chassis::rotate(double angle) {
	leftMotor1->Set(0.4);
	leftMotor2->Set(0.4);
	rightMotor1->Set(0.4);
	rightMotor2->Set(0.4);
}
