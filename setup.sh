#!/bin/bash

# create dir if it doesn't exist
sudo mkdir -p /etc/task

# create the file if it doesn't exist
sudo touch /etc/task/savedata.txt

# change ownership of the dir and file to the current user
sudo chown -R $USER:$USER /etc/task

# set the file permissions
sudo chmod 600 /etc/task/savedata.txt

echo "Setup complete!"
