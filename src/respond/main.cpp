#include<iostream>
#include<fstream> // per leggere i file json

#include<sstream> // per stampare la data
#include<ctime>
#include<iomanip>

#include<boost/function.hpp>

#include "ros/ros.h"
#include "std_msgs/String.h"

#include "include/json/json.hpp"
using json = nlohmann::json;

#include "include/responders/traj_responder.hpp"

// se riusciamo: migliorare implementazione
class Time_formatter {
public:
    static std::string curr_time_str() {
	auto t = std::time(nullptr);
	auto tm = *(std::localtime(&t));
	std::stringstream ss;
	ss<<std::put_time(&tm, "%d-%m-%Y-%H_%M_%S");
	return ss.str();
    }
};

class Logger {
public:
    Logger(std::string base_dir) {
	std::string s =
	    base_dir + "/"
	    + Time_formatter::curr_time_str()
	    + "-errors.log";
	ofs = std::ofstream(s);
    }
    void log(std::string msg) {
	ofs<<msg<<", tempo : " + Time_formatter::curr_time_str()<<std::endl;
    }
    ~Logger() {
	ofs.close();
    }
private:
    std::ofstream ofs;
};

Logger* logger;
Traj_responder* t;

void error_callback(const std_msgs::String::ConstPtr& msg) {
    std::string str = msg->data;
    logger->log(str);
    t->respond_to(str);
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "respond");
    ros::NodeHandle nh;

    std::string logs_dir;
    nh.getParam("/respond_node/logs_dir", logs_dir);
    logger = new Logger(logs_dir);

    std::string err;
    if(!nh.getParam("/respond_node/error_topic", err)) {
	std::cout<<"error_topic ERRORE"<<std::endl;
    }

    std::string err_json;
    if(!nh.getParam("/respond_node/error_json", err_json)) {
	std::cout<<"error_json ERRORE"<<std::endl;
    }

    t = new Traj_responder(err_json);

    ros::Subscriber error_sub = nh.subscribe(err, 100, error_callback);

    ros::spin();

    return 0;
}
