#pragma once

#include <QWidget>
#include "profile_editor.h"
#include "ui_edit_amneziawg.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class EditAmneziaWG;
}
QT_END_NAMESPACE

class EditAmneziaWG : public QWidget, public ProfileEditor {
    Q_OBJECT

public:
    explicit EditAmneziaWG(QWidget *parent = nullptr);

    ~EditAmneziaWG() override;

    void onStart(std::shared_ptr<Configs::Profile> _ent) override;

    bool onEnd() override;

private:
    Ui::EditAmneziaWG *ui;
    std::shared_ptr<Configs::Profile> ent;
};

