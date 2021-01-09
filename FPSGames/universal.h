#pragma once

/*
	universal.h
	Func:	用于定义全局变量，主要是为了解决contorl.h中的mouseButton_callback的鼠标检测功能
*/

extern int MouseEvent;//控制开火以及枪动作
extern int Movecount;
extern int BodyEvent;//控制相机下蹲
extern int Bodycount;
extern int Shootcount;//控制连射
extern int AimCount;	//	控制开镜
extern int ReturnHip;
extern float SpeedAdjust;//控制下蹲全局速度