all: gcc-10-cross linux-ci

.PHONY: gcc-10-cross
gcc-10-cross: gcc-10-cross/Dockerfile
	docker build -t alanif/gcc-10-cross gcc-10-cross
	docker push alanif/gcc-10-cross

.PHONY: linux-ci
linux-ci: gcc-10-cross linux-ci/Dockerfile
	docker build -t alanif/linux-ci linux-ci
	docker push alanif/linux-ci
