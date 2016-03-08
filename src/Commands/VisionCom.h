/*
 * VisionCom.h
 *
 *  Created on: Feb 19, 2016
 *      Author: James
 */

#ifndef VISIONCOM_H_
#define VISIONCOM_H_
#include "Commands/Command.h"
#include "WPILIB.h"
class VisionCom:public Command{

private:
	static std::shared_ptr<CANTalon> visionLeftMotor1;
	static std::shared_ptr<CANTalon> visionLeftMotor2;
	static std::shared_ptr<CANTalon> visionRightMotor1;
	static std::shared_ptr<CANTalon> visionRightMotor2;
	static std::shared_ptr<NetworkTable> contours;
	static void turnLeft(double);
	static void turnRight(double);
	static void stopTurning();
	static bool aligned=false;
	bool counterclockwise=false;
	static const int width=320, height=240,imageCenterX=width/2-8,looseTolerance=8,tightTolerance=3,maxDistance=30;
	static constexpr double slow=.1,fast=.2;
public:
	VisionCom();
	static void alignBot();
	void setLeft(bool);
	static void startGrip();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};


#endif /* SRC_COMMANDS_VISIONCOM_H_ */
