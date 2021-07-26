# Philosophers

3 programs to solve the [dining philosophers problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem)

Specifications:

- The status printed should not be scrambled or intertwined with another philosopher’s status.
- You can’t have more than 10 ms between the death of a philosopher and when it will print its death.
- Allowed tools:
  - philo_one: solve the problem with mutexes
  - philo_two: solve the problem with semaphores
  - philo_three: solve the problem with multi-threading and semaphores

Each program will display on the console the status of each philosophers:

- timestamp_in_ms X has taken a fork
- timestamp_in_ms X is eating
- timestamp_in_ms X is sleeping
- timestamp_in_ms X is thinking
- timestamp_in_ms X died

## Context

This project was done as part of 42's Software Engineer program.

## Usage

### Launch the program

Use the Makefile to compile each program

```bash
make
```

Each program takes 4 arguments:

- arg1: number of philosophers
- arg2: time_to_die in milliseconds
- arg3: time_to_eat in milliseconds
- arg4: time_to_sleep in milliseconds

To launch each program (philo_one for example):

```bash
./philo_one arg1 arg2 arg3 arg4
```

Common tests are:

- ./philo_nb 5 800 200 200 => philosophers should never die
- ./philo_nb 4 410 200 200 => philosophers should never die
- ./philo_nb 4 310 200 200 => philosophers should die
- ./philo_nb 2 410 200 200 => philosophers should never die

## Known Issues

The programs were tested successfully on Ubuntu Virtual Machine but philosophers tend to die prematurely on different machines for the (4 410 200 200) test.
