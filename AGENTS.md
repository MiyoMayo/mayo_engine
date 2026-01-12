# mayo: Agent Guidelines

## 目的
- 個人の趣味開発用 C++ゲームエンジン
- 優先：シンプルさ / 明示性 / 反復速度

## 前提
- C++23 / clang / CMake + Ninja
- 対象：M1+ macOS
- 主要依存：SDL3

## ディレクトリと公開境界
- Public API: `src/public/`
- 実装: `src/private/`
- 公開ヘッダ（`src/public/`）から `src/private/` を include しない
- install では public のみ `include/` に入る想定

## エラー処理
- 例外：基本使わない（未捕捉例外はバグ）
- ロジック系：Result/expected で返す
- 続行できる問題：ログで記録（エンジン層）

## テスト
- 1ファイル = 1 executable = 1 CTest unit
- 追加は `tests/CMakeLists.txt` の `add_unit_test(<name>.cpp)` を使う
- 成功は `return 0`、失敗は `return != 0`
- `assert` は tests のみ許可（`-UNDEBUG` 適用済み）

## フォーマット / 静的解析
- clang-format: `.clang-format` を参照
- clang-tidy: `.clang-tidy` を参照

## よく使うコマンド
- ビルド/テスト/インストールは README の **コマンド** セクションを参照
