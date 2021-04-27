pipeline {
    agent any

    stages {
        stage('Unittests') {
            steps {
                  sh 'make unit'
            }
        }
        stage('Build') {
            steps {
                  sh 'make'
            }
        }
        stage('Test') {
            steps {
                  sh 'make test'
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