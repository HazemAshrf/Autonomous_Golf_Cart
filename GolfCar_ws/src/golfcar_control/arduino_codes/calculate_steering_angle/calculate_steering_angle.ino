#include <Arduino.h>

const double ALPHA = 0.1;  // Smoothing factor for the low-pass filter

double calculate_steering_angle(double v_FL, double v_FR, double L, double D);

void setup() {
    Serial.begin(115200);
    Serial.println("Hello, world!");

    double v_FR = 5;  // Angular velocity of front right wheel
    double L = 2;      // Distance between the center of the front wheels and the center of the rear wheels
    double D = 0.75;   // Distance between the two rear wheels

    double filtered_steering_angle = 0;  // Initial filtered steering angle

    for (double v_FL = 4; v_FL <= 6; v_FL += 0.05) {
        double steering_angle = calculate_steering_angle(v_FL, v_FR, L, D);
        
        // Apply the low-pass filter
        filtered_steering_angle = ALPHA * steering_angle + (1 - ALPHA) * filtered_steering_angle;

        Serial.print("v_FL: ");
        Serial.print(v_FL);
        Serial.print(", Steering angle (degrees): ");
        Serial.print(steering_angle);
        Serial.print(", Filtered angle (degrees): ");
        Serial.println(filtered_steering_angle);
    }
}

void loop() {
    // Nothing to do in the loop for this example
}

double calculate_steering_angle(double v_FL, double v_FR, double L, double D) {
    // Calculate the ratio of angular velocities of front wheels
    double c = v_FL / v_FR;
    if (c == 1) {
        return 0.0;
    }
    
    // Coefficients of the quadratic equation
    double a = 1 - pow(c, 2);
    double b = 2 * D;
    double c_quad = pow(D, 2) + pow(L, 2) - pow(L, 2) * pow(c, 2);

    // Calculate the discriminant
    double discriminant = pow(b, 2) - 4 * a * c_quad;

    // Calculate x using the quadratic formula
    double x;
    if (discriminant >= 0) {
        double x1 = (-b + sqrt(discriminant)) / (2 * a);
        double x2 = (-b - sqrt(discriminant)) / (2 * a);
        
        // Choose the positive value of x
        x = max(x1, x2);
    } else {
        // No real solution for x
        return 0; // Or some default value
    }

    // Calculate the radius of the instantaneous center of rotation (ICR)
    double R_ICR = x + D / 2;

    // Calculate the steering angle
    double phi = atan(R_ICR / L);

    // Convert the angle to degrees
    double phi_degrees = 90 - degrees(phi);

    if (phi_degrees > 90) {
        phi_degrees = -(phi_degrees - 90);
    }

    return -radians(phi_degrees);
}
