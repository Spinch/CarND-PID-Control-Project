#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main()
{
  uWS::Hub h;

  PID pid;
  // TODO: Initialize the pid variable.
//   std::vector<double> k = {2.5, 0., 0.};
//   std::vector<double> kd = {0.1, 0.1, 0.001};
  std::vector<double> k = {2.86, 0.58, 0.003};
  std::vector<double> kd = {0.116232, 0.129146, 0.00105666};
  pid.Init(k[0], k[2], k[1]);

  h.onMessage([&pid, &kd, &k](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_value;
          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */
	  
	  
// 	  static double bestErr = std::numeric_limits<double>::max();
// 	  static int stage = -1;
// 	  static int ci = 0;
// 	  const double maxT = 3000;
// 	  static double stepT = 0;
// 	  if (stepT > maxT) {
// 	      
// 	    double err = pid.AccumError();
// 	    std::cout << "Tried K: " << k[0] << " " << k[1] << " " << k[2] << "; Kd: " << kd[0] << " " << kd[1] << " " << kd[2] << ". Err: " << err << "; berr: " << bestErr << std::endl;
// 	    
// 	    if (stage == -1) {
// 		stage = 0;
// 		bestErr = err;
// 		k[ci] += kd[ci];
// 	    }
// 	    else {
// 		if (err < bestErr) {
// 		    kd[ci] *= 1.1;
// 		    bestErr = err;
// 		    stage = 0;
// 		    ci = (ci+1)%3;
// 		    k[ci] += kd[ci];
// 		}
// 		else {
// 		    if (stage == 0) {
// 			k[ci] -= 2*kd[ci];
// 			stage = 1;
// 		    }
// 		    else if (stage == 1) {
// 			k[ci] += kd[ci];
// 			kd[ci] *= 0.9;
// 			ci = (ci+1)%3;
// 			k[ci] += kd[ci];
// 			stage = 0;
// 		    }
// 		}
// 	    }
// 	    pid.Init(k[0], k[2], k[1]);
// 	    std::string msg = "42[\"reset\",{}]";
// 	    ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
// 	      stepT = 0;
// 	  }
// 	  ++stepT;
	  
	  
	  pid.UpdateError(cte);
	  steer_value = pid.TotalError();
	  if (steer_value > 1)
	      steer_value = 1;
	  else if (steer_value < -1)
	      steer_value = -1;
          
          // DEBUG
//           std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = 0.1;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
//           std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}

