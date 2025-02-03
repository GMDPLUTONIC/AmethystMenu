// this code needs to be fixed

#include "../../hacks.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/GJCommentListLayer.hpp>

class $modify(MyGJCommentListLayer, GJCommentListLayer) {
    bool init(BoomListView* listView, char const* title, cocos2d::ccColor4B color, float width, float height, bool blueBorder) {
        if (!GJCommentListLayer::init(listView, title, color, width, height, blueBorder)) return false;

        if (Hacks::getInstance().isHackEnabled("april-fools-usernames")) {
            auto customListView = dynamic_cast<CustomListView*>(this->getChildByID("list-view"));
            if (customListView) {
                auto tableView = dynamic_cast<TableView*>(customListView->getChildByID("table-view"));
                if (tableView) {
                    auto commentsLayer = dynamic_cast<CCContentLayer*>(tableView->getChildByID("content-layer"));
                    if (commentsLayer) {
                        auto children = commentsLayer->getChildren();
                        for (int i = 0; i < children->count(); ++i) {
                            auto commentCell = dynamic_cast<CommentCell*>(children->objectAtIndex(i));
                            if (commentCell) {
                                auto mainMenu = commentCell->getChildByID("main-menu");
                                if (mainMenu) {
                                    auto userMenu = mainMenu->getChildByID("user-menu");
                                    if (userMenu) {
                                        auto usernameMenu = userMenu->getChildByID("username-menu");
                                        if (usernameMenu) {
                                            auto usernameButton = dynamic_cast<CCMenuItemSpriteExtra*>(usernameMenu->getChildByID("username-button"));
                                            if (usernameButton) {
                                                auto labelChild = dynamic_cast<CCLabelBMFont*>(usernameButton->getChildByID("label-id"));
                                                if (labelChild) {
                                                    labelChild->setString("Username");
                                                } else {
                                                    log::error("Unable to find CCLabelBMFont in username-button");
                                                }
                                            } else {
                                                log::error("Unable to find username-button");
                                            }
                                        } else {
                                            log::error("Unable to find username-menu");
                                        }
                                    } else {
                                        log::error("Unable to find user-menu");
                                    }
                                } else {
                                    log::error("Unable to find main-menu");
                                }
                            }
                        }
                    } else {
                        log::error("Unable to find CCContentLayer within TableView");
                    }
                } else {
                    log::error("Unable to find TableView in CustomListView");
                }
            } else {
                log::error("Unable to find CustomListView with ID 'list-view'");
            }
        }

        return true;
    }
};