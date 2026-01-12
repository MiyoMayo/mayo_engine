# MAYO ENGINE

楽しさ重視、趣味で作るM1+ mac向けゲームエンジン。  

## Stack

- SDL3.4
- C++23
- CMake4
- Ninja
- Clang21

# ビルド

## Configure

```sh
cmake --preset {debug/dev/release/tidy}
```

## Build

```sh
cmake --build --preset {debug/dev/release/tidy}
```

## Test

```sh
ctest --preset {debug/dev/release}
```

## Install

```sh
cmake --build --preset {install-debug/install-dev/install-release}
```

## Clean

```sh
rm -rf build/{debug/dev/release/tidy}
```

```sh
rm -rf staging/{debug/dev/release}
```

# コミットメッセージPrefix

- feat: 機能追加
- fix: バグ修正
- docs: READMEなどのドキュメント
- test: テスト追加/修正
- build: CMake/Presets/依存/ビルドシステム
- ci: GitHub ActionsなどのCI設定
- chore: 雑務(テンプレ更新、ツール設定、リネーム等)
- style: 整形のみ
- refactor: コード整理
