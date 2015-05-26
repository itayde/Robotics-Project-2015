#include <libplayerc++/playerc++.h>

using namespace std;
using namespace PlayerCc;

#define LASER_SAMPLES 666
#define LASER_CENTER 333
#define MIN_ANGLE_FOR_AVOIDANCE -45
#define MAX_ANGLE_FOR_AVOIDANCE 45
#define MAX_DIST_TO_OBSTACLE 0.35

/*
 * The laser proxy sensor measures the distance from 0 to 666
 * but we want to check only specific angles
 */
int deg_to_index(double deg)
{
	return (deg + 120) / 0.36;
}

int main(int argc, char** argv)
{
	PlayerClient pc("localhost",6665);
	Position2dProxy pp(&pc,0);
	LaserProxy lp(&pc, 0);

	while (true)
	{
		pc.Read();
		bool isObstacleInFront = false;
		int minIndex = deg_to_index(MIN_ANGLE_FOR_AVOIDANCE);
		int maxIndex = deg_to_index(MAX_ANGLE_FOR_AVOIDANCE);
		double whichSideTurnTo = 0;

		// go over the laser sensor and calculate which way the robot should drive
		for (int i = minIndex; i <= maxIndex; i++)
		{
			// if the laser detects an obstacle that is closer than MAX_DIST_TO_OBSTACLE(0.35 meters)
			// it calculates where the robot should turn in order to avoid the obstacle
			if (lp[i] < MAX_DIST_TO_OBSTACLE)
			{
				if (!isObstacleInFront)
					isObstacleInFront = true;

				// left side of the laser sensor - we add the distance to the variable
				if (i < LASER_CENTER)
				{
					whichSideTurnTo += lp[i];
				}
				// right side of the laser sensor - we Subtract the distance from the variable
				else if (i > LASER_CENTER)
				{
					whichSideTurnTo -= lp[i];
				}
			}
		}

		if (isObstacleInFront) {
			cout << whichSideTurnTo << endl;

			// if we have positive value it means that the laser sensor
			// detected that the right side of the robot is more "dangerous" so we turn left
			// else we turn right
			if (whichSideTurnTo > 0)
				pp.SetSpeed(0, 0.5);
			else
				pp.SetSpeed(0, -0.5);
		}
		else
			pp.SetSpeed(0.5, 0);
	}

	return 0;
}
