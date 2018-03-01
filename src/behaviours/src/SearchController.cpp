#include "SearchController.h"
#include <angles/angles.h>
#include <iostream>
SearchController::SearchController() {
 // rng = new random_numbers::RandomNumberGenerator(); /* commented out random*/
  currentLocation.x = 0;
  currentLocation.y = 0;
  currentLocation.theta = 0;

  centerLocation.x = 0;
  centerLocation.y = 0;
  centerLocation.theta = 0;
  result.PIDMode = FAST_PID;

  result.fingerAngle = M_PI/2;
  result.wristAngle = M_PI/4;
}

void SearchController::Reset() {
  result.reset = false;
}

/**
 * This code implements a basic random walk search.
 */
Result SearchController::DoWork() {

  if (!result.wpts.waypoints.empty()) {
    if (hypot(result.wpts.waypoints[0].x-currentLocation.x, result.wpts.waypoints[0].y-currentLocation.y) < .10) {//.10
      attemptCount = 0;
    }
  }

   if (attemptCount > 0 && attemptCount < 5) {
    attemptCount++;
    if (succesfullPickup) {
      succesfullPickup = false;
      attemptCount = 1;
    }
    return result;
  }
   else if(attemptCount >= 5 || attemptCount == 0) 
  {
    attemptCount = 1;


    result.type = waypoint;
    Point  searchLocation;

    //select new position 50 cm from current location
    if (first_waypoint)
    {
      first_waypoint = false;
      searchLocation.theta = currentLocation.theta + M_PI;
      searchLocation.x = currentLocation.x + (3.4 * cos(searchLocation.theta));//.5
      searchLocation.y = currentLocation.y + (0 * sin(searchLocation.theta));
    }
    else if (second_waypoint)
    {
      second_waypoint = false;
      //select new heading from Gaussian distribution around current heading
      searchLocation.theta = 1.5708; /*45 degrees in radians : rng->guassian : .785398 to 90 degress*/
      searchLocation.x = currentLocation.x + ( 0  * cos(searchLocation.theta));//.5
      searchLocation.y = currentLocation.y + ( 4.9  * sin(searchLocation.theta));
    }
    else if (third_waypoint)
    {
      third_waypoint = false;
      searchLocation.theta = M_PI; /*45 degrees in radians : rng->guassian : .785398 to 90 degress*/
      searchLocation.x = currentLocation.x + ( -3.4 * cos(searchLocation.theta));//.5
      searchLocation.y = currentLocation.y + ( 0 * sin(searchLocation.theta));
    }
    else if (fourth_waypoint)
    {
      searchLocation.theta = -1.5708; /*45 degrees in radians : rng->guassian : .785398 to 90 degress*/
      searchLocation.x = 0.0; //currentLocation.x + (0 * cos(searchLocation.theta)).5
      searchLocation.y = 0.0; //currentLocation.y + (4 * sin(searchLocation.theta))
    }
    else;
    /*{
      searchLocation.x = currentLocation.x + (3.4 * cos(searchLocation.theta));//.5
      searchLocation.y = currentLocation.y + (0 * sin(searchLocation.theta));
    }*/
    //result.wpts.waypoints.clear();
    result.wpts.waypoints.insert(result.wpts.waypoints.begin(), searchLocation);
    
    return result;
  }
 
}

void SearchController::SetCenterLocation(Point centerLocation) {
  
  float diffX = this->centerLocation.x - centerLocation.x;
  float diffY = this->centerLocation.y - centerLocation.y;
  this->centerLocation = centerLocation;
  
  if (!result.wpts.waypoints.empty())
  {
  result.wpts.waypoints.back().x -= diffX;
  result.wpts.waypoints.back().y -= diffY;
  }
  
}

void SearchController::SetCurrentLocation(Point currentLocation) {
  this->currentLocation = currentLocation;
}

void SearchController::ProcessData() {
}

bool SearchController::ShouldInterrupt(){
  ProcessData();

  return false;
}

bool SearchController::HasWork() {
  return true;
}

void SearchController::SetSuccesfullPickup() {
  succesfullPickup = true;
}


