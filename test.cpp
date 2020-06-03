#include <iostream>
#include <vector>
#include "ear_clipper.h"


int main()
{
	/* testing triangulation */
	std::vector<int> test_result = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5 };
    std::vector<ear_clipper::vertex<float>> vertices = 
	{
		{23.0, -14.0 },
		{19.5, -7.7 },
		{13.3, -8.6 },
		{5.5, -11.0 },
		{6.5, -20.0 },
		{13.0, -23.0 }
	 };
	 ear_clipper::triangulation<float> triangulator(vertices);
	 std::vector<int> indices = triangulator.triangulate();


	 for (int i = 0; i < indices.size(); i++)
	 {
		 if (indices[i] != test_result[i])
		 {
			 std::cout << "Test failed!" << std::endl;
			 return 1;
		 }
	 }
	 std::cout << "Test passed!" << std::endl;
	 return 0;
}
