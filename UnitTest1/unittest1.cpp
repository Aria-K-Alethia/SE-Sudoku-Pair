#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SE-Sudoku/Sudoku.h"
#include "../SE-Sudoku/Sudoku.cpp"
#include "../SE-Sudoku/InputHandler.cpp"
#include "../SE-Sudoku/InputHandler.h"
#include "../SE-Sudoku/Output.cpp"
#include "../SE-Sudoku/Output.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:

		Sudoku sudoku;
		
		UnitTest1() {
		}

		TEST_METHOD(TestMethod1)
		{
			/*
				@overview:test the method Sudoku::solve
			*/
			int puzzle[81] = {
						  5,1,2,3,4,0,7,8,9,
						  7,8,9,5,1,2,3,4,6,
						  3,4,6,7,8,9,5,1,2,
						  2,5,1,0,3,4,9,6,7,
						  6,9,7,2,5,1,8,3,4,
						  8,3,4,6,9,7,2,5,0,
						  1,2,5,4,7,3,6,9,8,
						  9,6,8,0,2,5,4,7,3,
						  4,7,3,9,6,8,1,2,5 };
			int solution[81] = {
				5,1,2,3,4,6,7,8,9,
				7,8,9,5,1,2,3,4,6,
				3,4,6,7,8,9,5,1,2,
				2,5,1,8,3,4,9,6,7,
				6,9,7,2,5,1,8,3,4,
				8,3,4,6,9,7,2,5,1,
				1,2,5,4,7,3,6,9,8,
				9,6,8,1,2,5,4,7,3,
				4,7,3,9,6,8,1,2,5
			};
			int temp[81];
			bool ret;
			ret = sudoku.solve(puzzle,temp);
			Assert::AreEqual(ret, true);
			for (int i = 0; i < 81; ++i) {
				Assert::AreEqual(temp[i], solution[i]);
			}
		}

	};
}