# Docker images for Alan

By leveraging a Docker service we can get a consistent build environment for Continuous Integration.
In Jenkins you need to create a cloud that is using the `alanif/linux-ci` docker image that can be created from this directory.

## Building the image

The `alanif/linux-ci` docker image should be available from dockerhub.io, but if it's not or it needs to be updated, issue the following command

    $ make

Tweak the Dockerfiles in the subdirectories as required.

## Your personal Docker cloud in Jenkins

In Jenkins, go to Manage Jenkins->Manage Nodes->Configure Clouds and add a new Docker cloud.
Depending on how you have your Docker service setup on the host fill out the Docker Host URI in the Docker Cloud settings.
Add a Docker template that has a label that you also use in the Jenkins job to restrict where the build can be run.
The template should use the image `alanif/linux-ci` and the Remote File System Root should be `/home/jenkins`.

## Connecting to the repo

For just compiling just anonymous cloning will work fine.

## Pushing build tags

For this you need the Jenkins plugin Git Publisher.
The tag should be `Build$BUILD_NUMBER` and the "Target remote name" `origin`.

You also need credentials to allow you to push.
(I'm still not sure about how Jenkins credentials work, so I'll have to leave out the details for now...)
