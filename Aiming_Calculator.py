#Julian Brady | Aiming calculator | BR-80 Turret Project | 2026

#Given launcher position, target position, and launch speed, this script calculates the angles that a launcher must turn to accurately hit a target


#Importing modules
import math
import matplotlib.pyplot as plt



####################~~~ Functions ~~~###################
#plot_trajectory() takes in the angle, time of flight, launch speed, and total initial height to plot the trajectory of the projectile
def plot_trajectory(title_addition, angle, time_of_flight, launch_speed, total_initial_height):
    time_intervals = 10
    time_step = time_of_flight/time_intervals
    x_values = []
    y_values = []
    for i in range(time_intervals + 1):
        t = i * time_step
        x = launch_speed * math.cos(angle) * t
        y = launch_speed * math.sin(angle) * t - 4.9 * t**2 + total_initial_height
        x_values.append(x)
        y_values.append(y)
    
    #Plotting the trajectory
    plt.plot(x_values, y_values, marker='^', color='blue')
    plt.xlabel("Horizontal Distance (m)")
    plt.ylabel("Vertical Distance (m)")
    plt.title(title_addition +"Trajectory of Projectile")
    plt.show()

####################~~~           ~~~###################



###########     1. Setup      ###########
### Constant Values ####
initial_horizontal_angle = 0 #horizontal angle (degrees)
    #the initial horizontal angle will always be zero because the final angle is measured with respect to the initial position of the launcher
launcher_height = 25 #launcher height (in centimeters), measured from the base of the launcher to the point where the barrel pivots
launch_speed = 42 #in meters/second


### Base Values ###
initial_height = float(input("Height of launcher base measured from ground (in meters): ")) #(meters)
initial_vertical_angle = float(input("Initial veritcal angle, measured from the horizontal (in degrees): ")) #vertical angle (degrees)


### Conversions and Summing ###
launcher_height  = launcher_height*0.01 #converting to meters
total_initial_height = initial_height + launcher_height #combines all values for the true intial height
initial_vertical_angle = initial_vertical_angle*(3.14159/180) #converting to radians
initial_horizontal_angle = initial_horizontal_angle*(3.14159/180) #converting to radians


### Target Values ###
#--Hybrid--#
#Obtaining horizontal angle, distance, and final height
final_horizontal_angle = float(input("Horizontal angle of target (measured counter-clockwise from barrel), in degrees: "))*(3.14159/180) #converting to radians after input
final_height = float(input("Height of target measured from ground (in meters): "))
horizontal_displacement = float(input("Target's distance from the launcher (in meters): ")) #distance of direct path to target

#Solving for vertical displacement
vertical_displacement = final_height - total_initial_height



###########     2. Calculations       ###########
### Base Calculations ###
if(vertical_displacement == 0):
    #Using range equation
    final_vertical_angle_1 = math.asin((9.8*horizontal_displacement)/(launch_speed**2))/2
    final_vertical_angle_2 = math.pi/2 - final_vertical_angle_1
    pass
else:
    #Quadratic formula to solve for tan(final_vertical_angle)
    a = (((-4.9)*(horizontal_displacement**2))/(launch_speed**2))
    b = horizontal_displacement
    c = a-vertical_displacement

    #Checking determinant
    if((b**2)-4*a*c < 0): #exiting script if determinant is negative, meaning no real solutions for final angle
        print("Error! No real solutions for the final angle. Target is out of range or invalid. Please check your inputs and try again.")
        exit()
    else: #proceeding with calculations if determinant is non-negative
        x_positive = (-b+math.sqrt(b**2-4*a*c))/(2*a)
        x_negative = (-b-math.sqrt(b**2-4*a*c))/(2*a)
        final_vertical_angle_1 = math.atan(x_positive)
        final_vertical_angle_2 = math.atan(x_negative)


### Assesing maximum and minimum final angles ###
#Calculation
higher_final_angle = max(final_vertical_angle_1, final_vertical_angle_2)
lower_final_angle = min(final_vertical_angle_1, final_vertical_angle_2)


## Plot Calculations ##
#Calculating time of flight for both angles
t_high_angle = horizontal_displacement/(launch_speed*math.cos(higher_final_angle))
t_low_angle = horizontal_displacement/(launch_speed*math.cos(lower_final_angle))

#Calculating max height of projectile for both angles
max_height_high_angle = (launch_speed**2)*(math.sin(higher_final_angle)**2)/(2*9.8)
max_height_low_angle = (launch_speed**2)*(math.sin(lower_final_angle)**2)/(2*9.8)


#Asking for user preference
high_or_low = str(input("Would you like to take the higher angle, " + str(math.degrees(higher_final_angle)) + " degrees, or the lower angle, " + str(math.degrees(lower_final_angle)) + " degrees? (H/L): "))
while (high_or_low != 'h' and high_or_low != 'H' and high_or_low != 'l' and high_or_low != 'L'): #error checking user input
    high_or_low = str(input("Would you like to take the higher angle, " + str(math.degrees(higher_final_angle)) + " degrees, or the lower angle, " + str(math.degrees(lower_final_angle)) + " degrees? (H/L): "))

if(high_or_low == 'h' or high_or_low == 'H'):
    #Taking higher angle
    final_vertical_angle = higher_final_angle
    t_chosen = t_high_angle

    #Storing alternative angle and time for plotting
    alternative_vertical_angle = lower_final_angle
    t_other = t_low_angle
elif(high_or_low == 'l' or high_or_low == 'L'):
    #Taking lower angle
    final_vertical_angle = lower_final_angle
    t_chosen = t_low_angle

    #Storing alternative angle and time for plotting
    alternative_vertical_angle = higher_final_angle
    t_other = t_high_angle




###########    3. Printing Results       ###########
vertical_angular_displacement = final_vertical_angle - initial_vertical_angle
print(" ")
print("The launcher must move " + str(math.degrees(vertical_angular_displacement)) + " degrees vertically and " + str(math.degrees(final_horizontal_angle)) + " degrees horizontally.")

#Graphical representation of trajectory
plot_trajectory("Chosen ", final_vertical_angle, t_chosen, launch_speed, total_initial_height)

#Asking user if they want to see the alternative trajectory
see_alternative = str(input("Would you like to see the trajectory for the alternative angle, " + str(math.degrees(alternative_vertical_angle)) + " degrees? (Y/N): "))
while(see_alternative != 'y' and see_alternative != 'Y' and see_alternative != 'n' and see_alternative != 'N'): #error checking user input
    see_alternative = str(input("Would you like to see the trajectory for the alternative angle, " + str(math.degrees(alternative_vertical_angle)) + " degrees? (Y/N): "))

if(see_alternative == 'y' or see_alternative == 'Y'):
    #Calling function to plot trajectory for alternative angle
    plot_trajectory("Alternative ", alternative_vertical_angle, t_other, launch_speed, total_initial_height)

