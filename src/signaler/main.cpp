#include<iostream>
#include<cstdlib> // per rand()

#include "ros/ros.h"
#include "std_msgs/String.h"

int main(int argc, char** argv) {
    ros::init(argc, argv, "signaler");
    ros::NodeHandle nh;

    std::string err;
    nh.getParam("error_topic", err);
    ros::Publisher error_pub = nh.advertise<std_msgs::String>(err, 100);
    ros::Rate hertz_sleeper(10);

    // qui ci vanno gli id delle funzioni, l'id identifica l'errore
    std::vector<std::string> errori = { "track lost",
					"camera fucked",
					"controls fucked",
					"everything fucked",
					"fire fucked" };
    while(ros::ok) {
	int ind = rand()%(errori.size());
	std_msgs::String messaggio;
	messaggio.data = errori[ind];

	ROS_INFO("%s", messaggio.data.c_str());
	error_pub.publish(messaggio);

	ros::spinOnce();
	hertz_sleeper.sleep();
    }

    return 0;
}
	

				     
