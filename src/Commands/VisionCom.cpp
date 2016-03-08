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
	aligned=false;
	if (contours==nullptr) {
		contours=NetworkTable::GetTable("GRIP/contoursReport");
	}
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


		if(centers.size()==0){				//If the box is not found
			printf("-----\n");
			stopTurning();
		}									//We're aligned inside the tighter space
		else if (abs(difference) <= tightTolerance)
		{
			printf("--|--\n");
			stopTurning();
		}									//We're to the left of the center(aka the goal)
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
		}									//We're past the center(aka to the right of the goal)
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
		}else{								//if we don't need to turn, we're aligned.
			aligned=true;
		}
	}else{
		printf("Table not found\n");
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
	USBCamera* cam=new USBCamera("cam0",false);  //This object's constructor opens camera for us

	cam->SetBrightness(50);
	cam->SetWhiteBalanceManual(whiteBalance::kFixedFluorescent1);
	cam->SetExposureAuto();
	//cam->UpdateSettings();
	//cam->OpenCamera();
	delete cam;

	std::system("/usr/local/frc/JRE/bin/java -jar /home/lvuser/grip.jar /home/lvuser/project.grip &");
}
void VisionCom::setLeft(bool isLeft=true){
	//Which direction to turn
	counterclockwise=isLeft;
}
void VisionCom::Initialize(){

	printf("vision initialized\n");
	//Keep turning until the contour is found
	while(contours->GetNumberArray("Area",llvm::ArrayRef<double>()).size()==0){
		turnRight(.5);
	}
	stopTurning();
}

void VisionCom::Execute(){
	printf("vision execute\n");
	//Try to turn towards goal
	alignBot();
	//Give it a moment to turn and then stop it
	Wait(.15);
	stopTurning();
	//Delay next runthrough to account for processing time
	Wait(.85);
}

void VisionCom::End(){
	//Stop motors when we finish with vision
	stopTurning();
}

bool VisionCom::IsFinished(){
	//End the command when the bot is aligned
	return aligned;
}

void VisionCom::Interrupted(){
	End();
}
