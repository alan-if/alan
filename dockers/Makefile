all: linux-ci

.PHONY: linux-ci
linux-ci: linux-ci/Dockerfile
	docker build -t alanif/linux-ci linux-ci
	docker push alanif/linux-ci
