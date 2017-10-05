#pragma once
/*
@overview:The class Output is NOT mutable.This class deals the task of generating error info on screen.
*/
class Output
{
public:
	Output();
	static void error(int code);
};