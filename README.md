📘 Readers–Writers Problem (Reader Priority)

This project is a simple simulation of the classical Readers–Writers synchronization problem using POSIX threads (pthreads) in C.

The implementation follows the reader-priority approach:

 • Multiple readers are allowed to read the shared resource at the same time.

 • A writer must wait until all active readers leave.

 • If readers keep coming, they block writers (this is normal for the reader-priority model).

This version is intentionally written in a student-friendly, easy-to-understand style with clear comments.



🧠 What the Program Does

The program creates:

 • several reader threads

 • several writer threads

Each thread:

 • waits for a random delay

 • attempts to read or write

 • enters the critical section following synchronization rules

 • prints what it is doing

 • sleeps to simulate reading/writing

 • leaves the critical section

You will see in the terminal how readers and writers interact.



🔧 How the Synchronization Works

We use two mutexes:

rc_mutex

Protects read_count, the number of active readers.

 • Ensures read_count++ and read_count-- happen safely.

wrt_mutex

Controls access to the shared resource:

 • Writers lock it for exclusive access.

 • The first reader locks it (to block writers).

 • The last reader unlocks it (to allow writers).

This creates reader-priority behavior.



📁 Project Structure

readers_writers.c     # The main program

README.md             # This file


▶️ How to Compile

Make sure you are in the directory where the file is located, then run:

gcc readers_writers.c -o rw -lpthread

This will create an executable named rw



▶️ How to Run

./rw

You will see output similar to:
[0.123] Reader 1 ENTERS (read_count=1)

[0.124] Reader 1 is READING...

[0.450] Writer 1 wants to WRITE

[1.124] Reader 1 LEAVES (read_count=0)

[1.125] Writer 1 STARTS WRITING
...


📚 Requirements

 • GCC
 
 • POSIX threads (Linux / macOS / WSL / MinGW)


 🙋‍♀️ Author Notes

The code includes detailed comments explaining:
 • entry sections
 • exit sections
 • why each mutex is used
 • what happens when readers/writers enter and leave

This README and code are designed to be clear even for someone seeing the Readers–Writers problem for the first time.