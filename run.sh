#!/usr/bin/env bash

set -e

# Переходим в каталог, где находится скрипт
cd "$(dirname "$0")"

# Проверяем доступность Docker
if ! sudo docker info >/dev/null 2>&1; then
    echo "Ошибка: Docker не запущен или недоступен."
    exit 1
fi

while true; do
    echo
    echo "========== TODO APP =========="
    echo "1. Собрать приложение"
    echo "2. Запустить базу данных"
    echo "3. Запустить приложение (интерактивно)"
    echo "4. Собрать и запустить всё"
    echo "5. Остановить контейнеры"
    echo "6. Показать состояние контейнеров"
    echo "0. Выход"
    echo "=============================="

    read -rp "Выбери действие: " choice

    case "$choice" in
        1)
            sudo docker compose build app
            ;;
        2)
            sudo docker compose up -d db
            ;;
        3)
            sudo docker compose up -d db
            sudo docker compose run --rm app
            ;;
        4)
            sudo docker compose build app
            sudo docker compose up -d db
            sudo docker compose run --build --rm app
            ;;
        5)
            sudo docker compose down
            ;;
        6)
            sudo docker compose ps -a
            ;;
        0)
            echo "Выход."
            exit 0
            ;;
        *)
            echo "Неверный выбор. Введи число от 0 до 6."
            ;;
    esac
done
