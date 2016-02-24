/*
 * VisionCom.cpp
 *
 *  Created on: Feb 19, 2016
 *      Author: James
 */
#include "VisionCom.h"
#include "../Robot.h"
std::shared_ptr<CANTalon> VisionCom::visionLeftMotor1=nullptr;
std::shared_ptr<CANTalon> VisionCom::visionLeftMotor2=nullptr;
std::shared_ptr<CANTalon> VisionCom::visionRightMotor1=nullptr;
std::shared_ptr<CANTalon> VisionCom::visionRightMotor2=nullptr;
VisionCom::VisionCom(){
	printf("vision constructed------------------------------\n");
	Requires(Robot::chassis.get());
	visionLeftMotor1=RobotMap::chassisLeftMotor1;
	visionLeftMotor2=RobotMap::chassisLeftMotor2;
	visionRightMotor1=RobotMap::chassisRightMotor1;
	visionRightMotor2=RobotMap::chassisRightMotor2;
	//Robot::chassis->getMotors(leftMotor1,leftMotor2,rightMotor1,rightMotor2);
}
std::shared_ptr<NetworkTable> VisionCom::contours=nullptr;

void VisionCom::alignBot(){
	if(contours!=nullptr){
		std::vector<double> centers=contours->GetNumberArray("centerX",llvm::ArrayRef<double>());
		//			int ctr=0;
		//			for(double x:centers)
		//			{
		//				printf("x at %d : %f",ctr++,x);
		//			}
		int centerX=centers.size()>0?centers.at(0):imageCenterX;

		int difference = imageCenterX-centerX;
		printf("difference: %d\n",difference);

		if(centers.size()==0){
			printf("-----\n");
			stopTurning();
		}
		else if (abs(difference) <= tightTolerance)
		{
			printf("--|--\n");
			stopTurning();
		}
		else if (difference<-tightTolerance)
		{
			if(difference >=-looseTolerance)
			{
				printf("-|---\n");
				turnLeft(slow);
			}
			else
			{
				printf("|----\n");
				turnLeft(fast);
			}
		}
		else if(difference>tightTolerance)
		{
			if(difference<=looseTolerance)
			{
				printf("---|-\n");
				turnRight(slow);
			}
			else
			{
				printf("----|\n");
				turnRight(fast);
			}
		}
	}else{
		printf("Table not found\n");
		contours=NetworkTable::GetTable("GRIP/contoursReport");
	}
}

void VisionCom::stopTurning(){

	visionLeftMotor2->Set(0);
	visionLeftMotor1->Set(0);

	visionRightMotor2->Set(0);
	visionRightMotor1->Set(0);
}

void VisionCom::turnLeft(double speed){
	visionLeftMotor2->Set(-speed);
	visionLeftMotor1->Set(-speed);

	visionRightMotor2->Set(-speed);
	visionRightMotor1->Set(-speed);
}

void VisionCom::turnRight(double speed){

	visionLeftMotor2->Set(speed);
	visionLeftMotor1->Set(speed);
	visionRightMotor2->Set(speed);
	visionRightMotor1->Set(speed);
}

void VisionCom::startGrip(){
	USBCamera* cam=new USBCamera("cam0",false);  //This objects constructor opens camera for us

	cam->SetBrightness(50);
	cam->SetWhiteBalanceManual(whiteBalance::kFixedFluorescent1);
	cam->SetExposureAuto();
	//cam->UpdateSettings();
	//cam->OpenCamera();
	delete cam;

	std::system("/usr/local/frc/JRE/bin/java -jar /home/lvuser/grip.jar /home/lvuser/project.grip &");
}

void VisionCom::Initialize(){
	printf("vision initialized\n");
}

void VisionCom::Execute(){
	printf("vision execute\n");
	alignBot();
	Wait(.15);
	stopTurning();
	Wait(.85);
}

void VisionCom::End(){
	stopTurning();
}

bool VisionCom::IsFinished(){
	return false;
}

void VisionCom::Interrupted(){
	End();
}
