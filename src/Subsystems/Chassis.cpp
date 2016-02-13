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
    gShiftSolen = RobotMap::chassisGShiftSolen;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS

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
	float left = deadband(Robot::oi->getleftJoy()->GetY() * MotorDirect,0.05);
	float right = deadband(Robot::oi->getrightJoy()->GetY() * MotorDirect,0.05);

	SmartDashboard::PutNumber("Left Joy: ",left);
	SmartDashboard::PutNumber("Right Joy: ",right);

	driveMotors->TankDrive(left,right);

}

void Chassis::Stop(){
	driveMotors->StopMotor();
}

void Chassis::Compress() {
	compressor->SetClosedLoopControl(true);
	compressor->Start();
}

void Chassis::GShift() {
	if (!gShiftSolen->Get()) {
		gShiftSolen->Set(true);
	}
	else if (gShiftSolen->Get()) {
		gShiftSolen->Set(false);
	}
}

void Chassis::InvertMotors() {
	MotorDirect = MotorDirect * -1;
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
	SmartDashboard::PutNumber("Left Motor 1 Encoder Position: ",leftMotor1->GetEncPosition());
	SmartDashboard::PutNumber("Left Motor 2 Encoder Position: ",leftMotor2->GetEncPosition());
	SmartDashboard::PutNumber("Right Motor 1 Encoder Position: ",rightMotor1->GetEncPosition());
	SmartDashboard::PutNumber("Right Motor 2 Encoder Position: ",rightMotor2->GetEncPosition());

}
