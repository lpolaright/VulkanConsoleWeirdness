# VulkanConsoleWeirdness

So, a weirdness has occured when I started following the tutorial for intel.
I wanted to make a vulkan application, and in order to do that I needed to create a surface.
However, I am not proficient with c++ so I figured - A console window is a window right?
I can just use that.

Wrong.

Console windows don't have capabilities to be used a vulkan surface.
After a brief discussion on the discord, people have realised it might be a bug.

In any case, you can try reproducing the bug by building the above and running it.
If the following message appears:
```
Surface creation succeeded even though it should not have?
Surface capabilities will say it lost the surface:
vk::Result:-1000000000
```
Then you know you have that bug.

Generally it seems like surface creation passes, however, when querying the surface capabilities the application says it can't find a surface anymore.

##Also, never use a console window as a vulkan surface.