---
title: "GraphWar Line Generation Program"
summary: "Developed a Python program that automatically generates piecewise mathematical functions from user clicks to create precise projectile trajectories in the game Graph War."
categories: ["Projects"]
tags: [ "Programming", "Personal"]
#externalUrl: ""
#showSummary: true
date: 2025-06-25
draft: false
---
## Intro
[Graph War](https://store.steampowered.com/app/1899700/Graphwar/) is an artillery game where players use mathematical functions to launch projectiles at opponents. The trajectory of each shot is determined by the function you write, requiring extreme precision to avoid obstacles and hit targets. Manual function writing often leads to missed shots due to estimation errors.

To solve this, I developed a Python program that automatically generates precise piecewise functions based on user-clicked positions, ensuring accurate shots every time.
<!--
        <div class="video-container">
            <video controls>
                <source src="https://immelwaylon.github.io/PersonalSite/media/curves.mp4" type="video/mp4">
                <source src="/PersonalSite/media/curves.mp4" type="video/mp4">
                Your browser does not support the video tag.
            </video>
        </div>
*Gameplay footage courtesy of [hueyfragic](https://www.youtube.com/@hueyfragic/featured)*
-->

## Line Generation Algorithm
The solution uses opposing absolute value functions (e.g., |x+1| - |x|) to create custom slopes on specific intervals while keeping other sections flat. By combining these functions, the program constructs a piecewise function that passes exactly through user-specified points given the correct y-offset. 

Here is a [desmos graph](https://www.desmos.com/calculator/s95kprka39) that demonstrates the concept. 

Since Graph War projectiles always originate from the player character, no y-offset calculations are needed—only relative slopes between points

## Development Process
1. **Prototyping**: Built initial line generation algorithm and validated with Desmos graphing
2. **Input Handling**: Implemented mouse input capture 
3. **Coordinate Conversion**: Created screen-to-Cartesian coordinate transformation 
4. **Edge Case Handling**: Added overlap resolution by minimally offsetting duplicate x-values
5. **Integration**: Combined all components into a functional program with terminal instructions

The most challenging aspect was implementing multithreading to manage simultaneous user input and timeout functionality.

## Final Code
[View full code on GitHub](https://github.com/immelwaylon/GraphWar-Line-Gen/tree/main)
<details>
<summary>Main Control Loop</summary>
        
```python


stopCommand = False
#Repeat program until asked to stop
while (not(stopCommand)):
    #Get mouse input
    mouse_input()
    #Sort points
    pixelList.sort()
    #Translate points to scaled cartesian coordinates
    finalList = translate_points(xScale, yScale, corner1, corner2)
    #Fix X coordinate repetition
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
</details>

## Results
The program successfully generates accurate mathematical functions that enable perfect shots in Graph War. These trajectories would be virtually impossible to calculate manually during gameplay.
<div style="display: flex; gap: 10px;">
  <img src="/PersonalSite/media/GraphWar/GraphWar1.png" alt="Front" style="width: 49%;">
  <img src="/PersonalSite/media/GraphWar/GraphWar2.png" alt="Top Left" style="width: 49%;">  
</div>






