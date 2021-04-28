pipeline {
    agent any
    triggers {
        githubPush()
    }
    stages {
       stage('Unittests') {
            steps {
                  sh 'make UNITOUTPUT="--xml TEST" unit'
            }
        }
        stage('Build') {
            steps {
                  sh 'make make BUILDNUMBER=$BUILD_NUMBER build'
            }
        }
        stage('Test') {
            steps {
                  sh 'make JREGROUTPUT=-xml test'
                  junit '**/*.xml'
            }
        }
        stage('Cross') {
            steps {
                  sh 'make BUILDNUMBER=$BUILD_NUMBER cross'
            }
        }
        stage('Package') {
            steps {
                  sh 'make BUILDNUMBER=$BUILD_NUMBER package'
                  sh 'make BUILDNUMBER=$BUILD_NUMBER cross-package'
                  archiveArtifacts artifacts: '*.tgz'
            }
        }
    }
}