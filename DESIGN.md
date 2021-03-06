# Design

## Overiew
![Overview](/images/JOE_Overview.png "overview")

## Goals

* Simplified tool chain
  * Java compiler
  * Java JRE
  * Java utilities

* Metacircular; all Java code
* Small
* Scalable
* Java interface to hardware
* Self hosting
* Fast build times
* Easy configuration

## High Level

JOE consists of the following major components: JikesRVM, boot-image-write, MMTK, GNU Classpath, and JAM (Java-on-Any-Machine).

### JikesRVM

Contains the JikesRVM code which includes the compilers, classloader, assemblers, processor architecture, library interfaces, vm magic, runtime, scheduler, and object model classes. These classes are located in jikesrvm-hg/rvm/.

### MMTK

This component provides the memory management subsystem for JikesRVM. The code is located in jikesrvm-hg/MMTK/src/.

### boot-image-writer

The boot-image-writer is a Java program that loads the JikesRVM classes  into a source JVM and packages them into boot image. It instantiates the JikesRVM objects in the source JVM, translates that object model into the JikesRVM object model and writes them out into a bootable image. It located in jikesrvm-hg/tools/bootImageWriter/src/.

### GNU Classpath

This component is an implementation of Java’s standard library and is located in classpath-0.98/.

### JAM

This component contains the computer hardware and processor interfaces and is located in jam/.		
