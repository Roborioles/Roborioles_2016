// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "PhotonCannonTog.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

PhotonCannonTog::PhotonCannonTog(): Command() {
        // Use requires() here to declare subsystem dependencies
    // eg. requires(chassis);
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	Requires(Robot::pneumaticSub.get());
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
}

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

// Called just before this Command runs the first time
void PhotonCannonTog::Initialize() {

}

// Called repeatedly when this Command is scheduled to run
void PhotonCannonTog::Execute() {
	//Robot::chassis->photonCannonTog();
	if (Robot::chassis->photonOn) {
		Robot::chassis->photonOn=false;
		Robot::chassis->GetPhotonCannon()->Set(Relay::kReverse);
		SmartDashboard::PutString("DB/String 5","Photon Off");
	} else {
		Robot::chassis->photonOn=true;
		Robot::chassis->GetPhotonCannon()->Set(Relay::kForward);
		SmartDashboard::PutString("DB/String 5","Photon On");
	}
}

// Make this return true when this Command no longer needs to run execute()
bool PhotonCannonTog::IsFinished() {
    return true;
}

// Called once after isFinished returns true
void PhotonCannonTog::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void PhotonCannonTog::Interrupted() {

}