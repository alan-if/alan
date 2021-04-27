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
                  sh 'make JREGROUTPUT=-xml UNITOUTPUT="--xml TEST" build'
            }
        }
        stage('Test') {
            steps {
                  sh 'make BUILDNUMBER=$BUILD_NUMBER JREGROUTPUT=-xml test'
                  junit '**/*.xml'
            }
        }
        stage('Deploy') {
            steps {
                  sh 'make BUILDNUMBER=$BUILD_NUMBER package'
                  archiveArtifacts artifacts: '*.zip'
            }
        }
    }
}