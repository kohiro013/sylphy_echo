# -----------------------------------------------------------------------------
# task runner makefile           ----------------------------------------------
# -----------------------------------------------------------------------------
.PHONY: help
help: ## makefileターゲット一覧表示
	@ echo ---- target list ----
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'

.PHONY: build
build: ## マイコン用elfファイル生成
	cmake -S Core -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=./arm-none-eabi-gcc_toolchain.cmake
	cmake --build build

.PHONY: clean
clean: ## buildディレクトリを消去
	rm -rf build

