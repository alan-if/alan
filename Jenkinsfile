pipeline {
    agent any
    triggers {
        githubPush()
    }
    stages {
        stage('Hello') {
            steps {
                sh 'echo "Hello World!"'
            }
        }
    }
}