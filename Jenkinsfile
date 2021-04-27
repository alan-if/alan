pipeline {
    agent any

    stages {
        stage('Unittests') {
            steps {
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
                  sh 'make BUILDNUMBER=$BUILD_NUMBER JREGROUTPUT=-xml test'
                  junit '**/target/*.xml'
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