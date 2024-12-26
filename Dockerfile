FROM ubuntu:22.04
RUN apt update
RUN apt install gcc make libglib2.0-dev libgtk2.0-dev valgrind \
libncurses-dev libncurses6 libncursesw6 libreadline8 \
libreadline-dev -y
RUN mkdir /project

# Copy your local `trabalho-pratico` folder to `/project/` in the container
COPY ../trabalho-pratico/ /project/

# Set the working directory
WORKDIR /project/