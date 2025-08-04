---
title: "GraphWar Line Generation Program"
summary: "Coded a python program to generate scaled piecewise equations that go through user-inputted click positions in the game Graph War."
categories: ["Projects"]
tags: [ "Programming", "Personal"]
#externalUrl: ""
#showSummary: true
date: 2025-06-25
draft: false
---
## Extended Summary
I created a python program that takes user mouse input and manipulates it to scaled generate piecewise functions in order to optomize graph war gaming.

Below is a video showing graphwar gameplay for reference. Credit to [hueyfragic](https://www.youtube.com/@hueyfragic/featured) for the video.
![]("/static/media/curves.mp4")

## The Basic Concept
The line generation works by a simple concept. If you offset two abs functions (one inverted) by making it abs(x+1)-abs(x), it will make a portion of the line sloped, while the rest remains flat. This means we can add these functions to make a line have whatever slopes we want on whatever intervals.  Here is a [desmos graph](https://www.desmos.com/calculator/s95kprka39) that demonstrates the concept between -1 and 1. We can use this to automate the generation of a line that goes through all the points that we specify. Normally, we would have to shift the line up or down, referencing something like a y intercept. However, the line in graph war always comes from a character, so we on't even have to set a y offset to go through the points we want. The relative slopes are all we need.

## My Process
1. Programmed a line generation script, tested with set points in desmos
2. Programmed a script to take mouse inputs based on terminal-printed instructions
3. Programmed a script to convert mouse(screen location) inputs to cartesian coordinates. 
4. I implemented an extra script to offset y values by a negligible amount if they overlapped, so a real line would be generated going roughly through every user-inputted point.
5. Integrated each function together for a final working script

I rewrote the entire thing from scratch 3-4 times trying to get it all working. 

## Final Code
[GitHub](https://github.com/immelwaylon/GraphWar-Line-Gen/tree/main)
<details>
<summary>Website Codeblock</summary>

```python
#Import libraries
import time
from threading import Thread
from pynput.mouse import Listener, Button

#Initialize global variables
global pixelList, corner1, corner2, rounding

#Setting Variables
clickTime = 30 #Default 30
xScale = 25  #Default 25
yScale = 15  #Default 15
rounding = 2 #Default 2


##---------------------------------------------Get user input---------------------------------------------##

#Initialize variables
pixelList = []
corner1 = []
corner2 = []

#Get user input
def mouse_input():
    global corner1, corner2
    keepCorner=False

    if (not(corner1 == [] and corner2 ==[])):
        if (input ("Pertain corner values? (y/n) ") == "y"):
            keepCorner=True
        else:
            keepCorner=False
            corner1 = []
            corner2 = []

    #Wait for user start
    input("Enter anything to start")

    if (keepCorner):
        print("\nClick pixel points")
    else:
        print("\nClick the top left corner")

    # Take click readings for <clickTime> seconds
    def on_click(x, y, button, pressed):
        #Bring global variables
        global pixelList, corner1, corner2

        # Check if the left button was pressed
        if pressed and button == Button.left:
            #Input corner values if not already
            if ((corner1 == []) and (keepCorner == False)):
                corner1 = (x,y)
                print("Click the bottom right corner")
            elif ((corner2 == []) and (keepCorner == False)):
                corner2 = (x,y)
                print("Click pixel points")
            #Input main list values
            else:
                pixelList.append((x,y))


    # Initialize the Listener to monitor mouse clicks 
    with Listener(on_click=on_click) as listener:
    #Thread to count <clickTime> seconds while listener is running
        def time_out(period_sec: int):
            time.sleep(period_sec)
            #Stop the listener
            listener.stop()
        Thread(target=time_out, args=(clickTime,)).start()
        listener.join()


##---------------------------------------------Convert user input to scaled coordinates---------------------------------------------##
  
#Modifies pixel point values to fit a specified cartesian plane
def translate_points(xScale, yScale, corner1, corner2):
    #Initialize variables
    global pixelList
    width = corner2[0] - corner1[0]
    height = corner2[1] - corner1[1]
    finalList = []
    i = 0

    #Translate points to a scaled cartesian plane
    while (i<len(pixelList)):
        #Translate points
        modX = (pixelList[i][0]-((corner1[0]+corner2[0])/2))
        modY = ((pixelList[i][1]-((corner1[1]+corner2[1])/2)) * -1)
        #Scale points
        modX *= (2*xScale)/width
        modY *= (2*yScale)/height
        
        finalList.append((modX, modY))
        i += 1    
    return finalList

#Fix X coordinate repitition
def discontinuity_fix(list):
    discontinuity = True

    while discontinuity == True:
        discontinuity = False
        i=1
        while i<((len(list))):
            if list[i-1][0] == list[i][0]:
                list[i] = (list[i][0] + (10**(-rounding)), list[i][1])
                discontinuity = True
            i += 1
        list.sort()
    return list


##---------------------------------------------Convert points to line and print---------------------------------------------##

#Output ABS function piecewise line
def output_abs_line(locations):
    xList = []
    yList = []

    for location in (locations):       
        xList.append(location[0])
        yList.append(location[1])
        
    slopeList = [0]*(len(xList))

    #Find all slopes
    for i in range (len(xList)-1):
        slopeList[i] = ((yList[i]-yList[i+1]) / (xList[i] - xList[i+1]))

    #Print equation of line going through points
    for i in range (len(xList)-1):
        slope = slopeList[i]
        x = xList[i]
        x2 = xList[i+1]
        
        if (slope<=0):
            print ("+ ((abs(" + str(round((slope), rounding)) + "x +" + str(round(x2*(abs(slope)), rounding)) + ")-abs(" + str(round((slope), rounding)) + "x +" + str(round(x*abs(slope), rounding)) + "))/2)", end='')
        else:
            print ("- ((abs(" + str(round((slope), rounding)) + "x +" + str(round(-x2*(abs(slope)), rounding)) + ")-abs(" + str(round((slope), rounding)) + "x +" + str(round(-x*abs(slope), rounding)) + "))/2)", end='')
        locations.clear()  # Clear the list after calculation

#Generate a smooth line going through the specified points
def generate_smooth_line():
    print("WIP")


##---------------------------------------------Execute Program---------------------------------------------##

stopCommand = False
#Repeat program until asked to stop
while (not(stopCommand)):
    #Get mouse input
    mouse_input()
    #Sort points
    pixelList.sort()
    #Translate points to scaled cartesian coordinates
    finalList = translate_points(xScale, yScale, corner1, corner2)
    #Fix X coordinate repitition
    finalList = discontinuity_fix(finalList)

    #Output
    for i in finalList:
        print(i)
    print()
    output_abs_line(finalList)

    #Reset old pixel data
    pixelList = []

    #Ask to stop
    stopCommand = (not(input("\nContinue? (y/n) ") == "y"))
```
</details>

## Resulting Line Generation
With this program, I was able to successfully generate accurate lines to easily win graph war games that would have otherwise been almost impossible.
<div style="display: flex; gap: 10px;">
  <img src="/media/GraphWar/GraphWar1.png" alt="Front" style="width: 49%;">
  <img src="/media/GraphWar/GraphWar2.png" alt="Top Left" style="width: 49%;">  
</div>
