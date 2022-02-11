# PomidoroTimer
A Qt utility to remind me of the time.  
It's easy to forget the work and then procrastinate while surfing the web or watching funny cat videos on Youtube. I developed an application to warn me hourly in C# and was using it on my Windows machines.
I've been working on Linux for my daily tasks and I use Windows only on virtual machines. I needed to use that application on my Ubuntu desktop but I couldn't write C# desktop applications on Linux.
So I decided to code the same application using Qt and C++. Qt is working on almost every CPU and OS. Problem solved :)  

## What does it do
It checks the beginning of an hour and pops up a window to remind me that the clock is running and I am procrastinating. I added a sound and a DND mode this time.  

Custom reminders such as _"turn off the pressure cooker at 17:15"_ can be added now.
