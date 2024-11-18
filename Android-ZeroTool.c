#!/data/data/com.termux/files/usr/bin/bash

# Android Termux Toolkit - BY RENC!

GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'

full_backup() {
    echo -e "${YELLOW}Создание полного бэкапа...${NC}"
    BACKUP_DIR="/sdcard/AndroidBackups/$(date +%Y%m%d_%H%M%S)"
    mkdir -p "$BACKUP_DIR"

    cp -r /data/data "$BACKUP_DIR/app_data"
    cp -r /sdcard/DCIM "$BACKUP_DIR/photos"
    cp -r /sdcard/Documents "$BACKUP_DIR/documents"

    pm list packages -3 > "$BACKUP_DIR/installed_apps.txt"
    
    echo -e "${GREEN}Бэкап создан в $BACKUP_DIR${NC}"
}

network_monitor() {
    echo -e "${YELLOW}Монитор сетевой активности...${NC}"

    netstat -tuln
    ip -s link show
}

clean_junk() {
    echo -e "${YELLOW}Очистка мусорных файлов...${NC}"

    find /sdcard -type f \( -name "*.tmp" -o -name "*.log" -o -name "*.cache" \) -delete
    
    rm -rf /sdcard/Download/*.apk
    rm -rf /sdcard/Download/*.zip
    
    echo -e "${GREEN}Мусорные файлы удалены${NC}"
}

disable_services() {
    echo -e "${YELLOW}Отключение ненужных служб...${NC}"

    services=(
        "bluetooth"
        "nfc"
        "location"
        "mobile_data"
    )
    
    for service in "${services[@]}"; do
        settings put global "$service"_disabled 1
    done
    
    echo -e "${GREEN}Службы отключены${NC}"
}

device_diagnostics() {
    echo -e "${YELLOW}Диагностика устройства...${NC}"
    
    echo "Версия Android: $(getprop ro.build.version.release)"
    echo "Модель устройства: $(getprop ro.product.model)"
    echo "Серийный номер: $(getprop ro.serialno)"
 
    free -h
    df -h
}

disable_camera() {
    echo -e "${YELLOW}Отключение камеры...${NC}"
    
    if ! command -v adb &> /dev/null; then
        echo -e "${RED}Ошибка: ADB не установлен. Установите через Termux:${NC}"
        echo "pkg install android-tools"
        return 1
    fi

    adb devices > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo -e "${RED}Подключите устройство и разрешите отладку USB${NC}"
        return 1
    fi

    adb shell cmd package disable-user --user 0 android/com.android.camera2
    echo -e "${GREEN}Камера отключена${NC}"
}

main_menu() {
    while true; do
        clear
        echo -e "${BLUE}===== ANDROID TOOLKIT  =====${NC}"
        echo -e "${BLUE}      RENC! @thiasoft${NC}"
        echo -e "${GREEN}1. Полный бэкап${NC}"
        echo -e "${GREEN}2. Монитор сетевой активности${NC}"
        echo -e "${GREEN}3. Очистка мусорных файлов${NC}"
        echo -e "${GREEN}4. Отключение служб${NC}"
        echo -e "${GREEN}5. Диагностика устройства${NC}"
        echo -e "${GREEN}6. Отключение камеры${NC}"
        echo -e "${RED}7. Выход${NC}"
        
        read -p "Выберите опцию (1-7): " choice
        
        case $choice in
            1) full_backup ;;
            2) network_monitor ;;
            3) clean_junk ;;
            4) disable_services ;;
            5) device_diagnostics ;;
            6) disable_camera ;;
            7) exit 0 ;;
            *) 
                echo -e "${RED}Неверный выбор. Попробуйте снова.${NC}"
                sleep 2
                ;;
        esac
        
        read -p "Нажмите Enter для продолжения..." pause
    done
}

main_menu