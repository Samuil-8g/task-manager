#pragma once
#include "Task.h"
#include "Project.h"
#include "User.h"
#include <string>
#include <vector>
#include <iostream>
 
// Един notification обект
struct Notification {
    std::string message;
    std::string  targetUser; // за кого е
    std::string createdAt;
 
    Notification(const std::string& msg, const std::string& user)
        : message(msg), targetUser(user), createdAt(currentDateTime()) {}
 
    std::string toString() const {
        return "[" + createdAt + "] @" + targetUser + ": " + message;
    }
};
 
class NotificationService {
private:
    std::vector<Notification> notifications;
 
public:
    // Известие при смяна на статус
    void onStatusChanged(Task* task, User* changedBy) {
        std::string msg = "Задача \"" + task->getTitle()
                        + "\" е сменена на " + taskStatusToString(task->getStatus());
        addNotification(msg, changedBy->getUsername());
        if (task->getAssignee() && task->getAssignee()->getId() != changedBy->getId()) {
            addNotification(msg, task->getAssignee()->getUsername());
        }
    }
 
    // Известие при нов коментар
    void onCommentAdded(Task* task, User* author) {
        std::string msg = "Нов коментар на \"" + task->getTitle()
                        + "\" от " + author->getUsername();
        if (task->getAssignee()) {
            addNotification(msg, task->getAssignee()->getUsername());
        }
    }
 
    // Проверява всички задачи за закъснели и генерира известия
    void checkOverdue(const std::vector<Project*>& projects) {
        for (auto p : projects) {
            for (auto t : p->getTasks()) {
                if (t->isOverdue()) {
                    std::string msg = "ЗАКЪСНЯЛА задача: \"" + t->getTitle() + "\"";
                    if (t->getAssignee()) {
                        addNotification(msg, t->getAssignee()->getUsername());
                    }
                }
            }
        }
    }
 
    // Известие при назначаване
    void onTaskAssigned(Task* task, User* assignee) {
        std::string msg = "Назначена ти е задача: \"" + task->getTitle() + "\"";
        addNotification(msg, assignee->getUsername());
    }
 
    // Показва всички известия за даден потребител
    void printNotifications(const std::string& username) {
        std::cout << "\n=== Известия за " << username << " ===\n";
        bool any = false;
        for (const auto& n : notifications) {
            if (n.targetUser == username) {
                std::cout << "  " << n.toString() << "\n";
                any = true;
            }
        }
        if (!any) std::cout << "  Няма известия.\n";
        std::cout << "================================\n\n";
    }
 
    // Показва всички известия
    void printAll() {
        std::cout << "\n=== Всички известия ===\n";
        if (notifications.empty()) {
            std::cout << "  Няма известия.\n";
        }
        for (const auto& n : notifications) {
            std::cout << "  " << n.toString() << "\n";
        }
        std::cout << "=======================\n\n";
    }
 
    int count() const { return (int)notifications.size(); }
 
private:
    void addNotification(const std::string& msg, const std::string& user) {
        notifications.emplace_back(msg, user);
        // Показва веднага в конзолата
        std::cout << "  [Известие] @" << user << ": " << msg << "\n";
    }
};