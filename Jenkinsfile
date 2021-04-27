pipeline {
    agent any

    stages {
        stage('Unittests') {
            steps {
                  sh 'echo $BUILD_NUMBER > BUILD_NUMBER'
                  sh 'make UNITOUTPUT="--xml TEST" unit'
            }
        }
        stage('Build') {
            steps {
                  sh 'make'
            }
        }
        stage('Test') {
            steps {
                  sh 'make JREGROUTPUT=-xml test'
                  junit '**/target/*.xml'
            }
        }
        stage('Deploy') {
            steps {
                  sh 'make package'
                  archiveArtifacts artifacts: '*.zip'
            }
        }
    }
}