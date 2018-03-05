/*!
   \image rtf mstar_logo.png
   \mainpage Utopia API specification
   
   <b>Purpose</b>

   This guide is for the developers, who intent to use MStar Device driver (Utopia) to achieve specific features or integrate into specific Application/Middleware interfaces. This guide describes the basic Utopia driver architecture and detail interface descriptions of modules.

   <b> Abbrevation </b>   
   
   There are the common abbreviations for specific module or well known terminology.
   \image html utopia_arch3.png

     \section arch  Architecture of Utopia

     <b> Unified Driver: </b> 
     - Utopia is MStar unified driver. The goals of unified driver are followings:
	-# Easy to port new middleware
	-# Easy to support other platform
	-# Easy to adopt to different chip
      And additional advantages:
	-# Secure driver main flow
	-# Keep field experience
	-# Utilize resource

      <b> Design Concept: </b>
      - A single driver supports a range of products and also supports multiple operation systems.
	-# Cross chip
	-# Cross-platform (CPU, OS, Toolchain)
        -#  nified interface
      \image html utopia_arch.png

      <b>Layers:</b>
      - Architecture of Utopia is divided into 4 layers.
        -# Driver: Provide unified interfaces. Each drivers are independent.
        -# Lower Driver: Provide system or lower common interfaces for Driver layer.
        -# HAL: Abstract HW functions.
        -# OS Wrapper: Wrap OS relative functions.

      \image html utopia_arch2.png 

*/


