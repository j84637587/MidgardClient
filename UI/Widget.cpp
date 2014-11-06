#include "Widget.h"
#include "Manager.h"

namespace UI
{
    Widget::Widget(uint32_t dwIdent, uint8_t uFlagMod, float x, float y, float w, float h)
    {
        vPos.x  = x;
        vPos.y  = y;
        vSize.x = w;
        vSize.y = h;
        pParent = NULL;
        uAlignH = LEFT;
        uAlignV = TOP;
        dwBorderWidth = 1;
        fCornerRadius = 5;
        pColors[FOREGROUND] = sf::Color::White;
        pColors[BACKGROUND] = sf::Color::Transparent;
        pColors[OUTLINE] = sf::Color::Black;
        uFlag = uFlagMod;
        dwID = dwIdent;
        UpdateLocation();
    }

    Widget::Widget(uint32_t dwIdent, uint8_t uFlagMod, sf::Vector2f vPos, sf::Vector2f vSize)
    {
        this->vPos = vPos;
        this->vSize = vSize;
        pParent = NULL;
        uAlignH = LEFT;
        uAlignV = TOP;
        dwBorderWidth = 1;
        fCornerRadius = 5;
        pColors[FOREGROUND] = sf::Color::White;
        pColors[BACKGROUND] = sf::Color::Transparent;
        pColors[OUTLINE] = sf::Color::Black;
        uFlag = uFlagMod;
        dwID = dwIdent;
        UpdateLocation();
    }

    Widget::~Widget()
    {
        for (WidgetList::iterator iter = lstChildren.begin(); iter != lstChildren.end(); iter++)
        {
            Widget* pWidget = *iter;
            delete pWidget;
        }
    }

    bool Widget::AttachTo(Widget *pWidget)
    {
        if (pParent != NULL) return false;
        pParent = pWidget;
        UpdateLocation();
        return true;
    }

    void Widget::Detach()
    {
        pParent = NULL;
        UpdateLocation();
    }

    void Widget::AddChild(Widget* pWidget, Widget* pBefore, Widget* pAfter)
    {
        if (pBefore == NULL && pAfter == NULL)
        {
            lstChildren.push_back(pWidget);
            pWidget->AttachTo(this);
            return;
        }
        for (WidgetList::iterator iter = lstChildren.begin(); iter != lstChildren.end(); iter++)
        {
            if (*iter == pBefore)
            {
                lstChildren.insert(++iter, pWidget);
                pWidget->AttachTo(this);
                break;
            }
            if (*iter == pAfter)
            {
                lstChildren.insert(iter, pWidget);
                pWidget->AttachTo(this);
                break;
            }
        }
    }

    void Widget::DelChild(Widget* pWidget, bool bCleanup)
    {
        if (bCleanup)
        {
            lstChildren.remove(pWidget);
            return;
        }
        for (WidgetList::iterator iter = lstChildren.begin(); iter != lstChildren.end(); iter++)
        {
            if (*iter == pWidget)
            {
                lstChildren.erase(iter);
                pWidget->Detach();
                break;
            }
        }
    }
    const Widget* Widget::GetParent() const
    {
        return pParent;
    }

    const Widget* Widget::GetChild(uint32_t dwNeeded) const
    {
        Widget* pWidget = NULL;
        for (WidgetList::const_iterator iter = lstChildren.cbegin(); iter != lstChildren.cend(); iter++)
        {
            pWidget = *iter;
            if (pWidget->GetID() == dwNeeded) break;
        }
        return pWidget;
    }

    const uint32_t Widget::GetID() const
    {
        return dwID;
    }

    const sf::Vector2f& Widget::GetPosition(bool bReal) const
    {
        if (bReal)
        {
            return vPosReal;
        }
        else
        {
            return vPos;
        }
    }

    void Widget::MoveTo(sf::Vector2f vPos)
    {
        this->vPos = vPos;
        UpdateLocation();
    }

    void Widget::MoveTo(float x, float y)
    {
        vPos.x = x;
        vPos.y = y;
        UpdateLocation();
    }

    void Widget::UpdateLocation()
    {
        if (pParent == NULL) return;
        vPosReal = vPos;
        switch(uAlignH)
        {
            case LEFT:   { vPosReal.x += pParent->GetPosition().x; break; }
            case MIDDLE: { vPosReal.x  = pParent->GetWidth()/2 - vSize.x/2 + vPosReal.x; break; }
            case RIGHT:  { vPosReal.x  = pParent->GetWidth()   - vSize.x   + vPosReal.x; break; }//x should be negative in this case else its stupid
        }
        switch(uAlignV)
        {
            case TOP:    { vPosReal.y += pParent->GetPosition(true).y; break; }
            case CENTER: { vPosReal.x  = pParent->GetHeight()/2 - vSize.y/2 + vPosReal.y; break; }
            case BOTTOM: { vPosReal.x  = pParent->GetHeight()   - vSize.y   + vPosReal.y; break; }//y should be negative in this case else its stupid
        }
    }

    const sf::Vector2f& Widget::GetSize() const
    {
        return vSize;
    }
    void Widget::Resize(sf::Vector2f vSize)
    {
        this->vSize = vSize;
        UpdateLocation();
    }
    void Widget::Resize(float w, float h)
    {
        vSize.x = w;
        vSize.y = h;
        UpdateLocation();
    }
    void Widget::SetWidth(float w)
    {
        vSize.x = w;
        UpdateLocation();
    }
    const float Widget::GetWidth() const
    {
        return vSize.x;
    }
    void Widget::SetHeight(float h)
    {
        vSize.y = h;
        UpdateLocation();
    }
    const float Widget::GetHeight() const
    {
        return vSize.y;
    }

    const HAlign Widget::GetHAlignment() const
    {
        return uAlignH;
    }
    void Widget::SetHAlignment(HAlign h)
    {
        uAlignH = h;
        UpdateLocation();
    }

    const VAlign Widget::GetVAlignment() const
    {
        return uAlignV;
    }
    void Widget::SetVAlignment(VAlign v)
    {
        uAlignV = v;
        UpdateLocation();
    }
    void Widget::SetAlignment(HAlign h, VAlign v)
    {
        uAlignH = h;
        uAlignV = v;
        UpdateLocation();
    }

    const sf::Color& Widget::GetColor(ColorID id) const
    {
        if (id >= MAXID) {return sf::Color::Black;}
        return pColors[id];
    }
    void Widget::SetColor(sf::Color& color, ColorID id)
    {
        if (id >= MAXID) {return;}
        pColors[id] = color;
    }
    const uint32_t Widget::GetBorderWidth() const
    {
        return dwBorderWidth;
    }
    void Widget::SetBorderWidth(uint32_t dwBorder)
    {
        dwBorderWidth = dwBorder;
    }
    const float Widget::GetCornerRadius() const
    {
        return fCornerRadius;
    }
    void Widget::SetCornerRadius(float fCorner)
    {
        fCornerRadius = fCorner;
    }
    //void ChangeZIndex(ZIndex z)//Not doing now - lets see
    //{
    //}

    const bool Widget::IsEnabled()   const { return isFlagSet(ENABLED); }
    const bool Widget::IsVisible()   const { return isFlagSet(VISIBLE); }
    const bool Widget::IsEditable()  const { return isFlagSet(EDITABLE); }
    const bool Widget::HasFocus()    const { return isFlagSet(INFOCUS); }
    const bool Widget::IsClickable() const { return isFlagSet(CLICKABLE); }
    const bool Widget::IsFocusable() const { return isFlagSet(FOCUSABLE); }

    void Widget::SetEnabled(  bool bStatus) { setFlag(ENABLED,  bStatus); }
    void Widget::SetVisible(  bool bStatus) { setFlag(VISIBLE,  bStatus); }
    void Widget::SetEditable( bool bStatus) { setFlag(EDITABLE, bStatus); }
    void Widget::SetFocus(    bool bStatus) { setFlag(INFOCUS,  bStatus); }

    bool Widget::ParseEvent(sf::Event event, Manager* pManager)
    {
        if (!IsVisible() || !IsEnabled()) { return false; }
        //Keyboard events first - Will only be sent to this widget if it has focus
        if (event.type == event.KeyPressed)
        {
            this->KeyPressed(event.key, pManager);
            if (event.key.code == sf::Keyboard::Tab)
            {
                if (event.key.shift)
                {
                    pManager->SwitchFocus(false);
                }
                else
                {
                    pManager->SwitchFocus(true);
                }
            }
        }
        if (event.type == event.KeyReleased)
        {
            this->KeyReleased(event.key, pManager);
        }
        if (event.type == event.TextEntered)
        {
            this->TextEntered(event.text, pManager);
        }

        //Mouse Events
        bool bFlag = false;//true if any of the children collected the event
        if (event.type == event.MouseButtonPressed && IsPointInside(event.mouseButton.x, event.mouseButton.y))
        {
            bFlag = SpreadEvent(event, pManager);
            if (bFlag) {return bFlag;}
            MousePressed(event.mouseButton, pManager);
            if (pManager->IsPressed(NULL) && IsClickable())
            {
                pManager->SetPressed(this, event.mouseButton.button);
            }
            if (IsFocusable())
            {
                pManager->SwitchFocus(this);
            }
            bFlag = true;
        }
        if (event.type == event.MouseButtonReleased && IsPointInside(event.mouseButton.x, event.mouseButton.y))
        {
            bFlag = SpreadEvent(event, pManager);
            if (bFlag) {return bFlag;}

            MouseReleased(event.mouseButton, pManager);
            if (pManager->IsPressed(this, event.mouseButton.button))
            {
                MouseClicked(event.mouseButton, pManager);
                pManager->SetPressed(NULL, event.mouseButton.button);
            }
            bFlag = true;
        }
        else if (event.type == event.MouseButtonReleased && pManager->IsPressed(this, event.mouseButton.button))
        {
            pManager->SetPressed(NULL, event.mouseButton.button);
            MouseReleased(event.mouseButton, pManager);
        }

        if (event.type == event.MouseMoved)
        {
            if (pManager->IsHovered(this) && !IsPointInside(event.mouseMove.x, event.mouseMove.y))
            {
                MouseLeft(event.mouseMove, pManager);
                pManager->SetHovered(NULL);
                return false;
            }
            else if (IsPointInside(event.mouseMove.x, event.mouseMove.y))
            {
                bFlag = SpreadEvent(event, pManager);
                if (bFlag) return bFlag;
                if (pManager->IsHovered(this))
                {
                    MouseMoved(event.mouseMove, pManager);
                }
                else
                {
                    MouseEntered(event.mouseMove, pManager);
                    pManager->SetHovered(this);
                }
                bFlag = true;
            }
        }
        //(event.type == event.MouseLeft) should be handled by Manager
        return bFlag;
    }

    sf::FloatRect Widget::GetBBox(bool bReal) const
    {
        sf::Vector2f vpos = GetPosition(bReal);
        return sf::FloatRect(vpos.x, vpos.y, vSize.x, vSize.y);
    }

    bool Widget::IsPointInside(uint32_t x, uint32_t y)
    {
        const sf::FloatRect rect = GetBBox(true);
        return rect.contains(x, y);
    }

    void Widget::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        for (WidgetList::const_iterator iter = lstChildren.cbegin(); iter != lstChildren.cend(); iter++)
        {
            Widget* pWidget = *iter;
            if (pWidget->IsVisible())
            {
                target.draw(*pWidget, states);
            }
        }
    }

    bool Widget::SpreadEvent(sf::Event event, Manager* pManager)
    {
        bool bFlag = false;
        for (WidgetList::reverse_iterator iter = lstChildren.rbegin(); iter != lstChildren.rend(); iter++)//Reverse Order because last one has highest preference since its drawn last
        {
            Widget *pWidget = *iter;
            bFlag = pWidget->ParseEvent(event, pManager);
            if (bFlag) break;
        }
        return bFlag;
    }

    void Widget::DrawBorder(sf::RenderTarget& target, sf::RenderStates states) const
    {
        static const uint32_t dwCount = 20;
        static const float fDeltaAngle = 90.0f/(dwCount-1);
        static const float fPI = 3.141592654f;
        static float fCenterX, fCenterY;

        sf::VertexArray vaBorder(sf::TrianglesStrip, dwCount*4*2 + 2);
        for (uint32_t i = 0; i < dwCount*4*2; i+=2)
        {
            uint32_t dwIndex = i/2;
            uint32_t dwCenterIndex = dwIndex/dwCount;
            switch (dwCenterIndex)
            {
                case 0: {fCenterX = vSize.x - fCornerRadius; fCenterY = fCornerRadius - vSize.y; break;}
                case 1: {fCenterX = fCornerRadius;           fCenterY = fCornerRadius - vSize.y; break;}
                case 2: {fCenterX = fCornerRadius;           fCenterY = 0 - fCornerRadius;       break;}
                case 3: {fCenterX = vSize.x - fCornerRadius; fCenterY = 0 - fCornerRadius;       break;}
            }
            vaBorder[i] = sf::Vertex(
                            sf::Vector2f(
                                vPos.x + fCornerRadius * cos(fDeltaAngle * (dwIndex-dwCenterIndex) * fPI/180.0) + fCenterX,
                                vPos.y + fCornerRadius * sin(fDeltaAngle * (dwIndex-dwCenterIndex) * fPI/180.0) - fCenterY),
                            GetColor(OUTLINE)
                        );
            vaBorder[i+1] = sf::Vertex(
                            sf::Vector2f(
                                vPos.x + (fCornerRadius-dwBorderWidth) * cos(fDeltaAngle * (dwIndex-dwCenterIndex) * fPI/180.0) + fCenterX,
                                vPos.y + (fCornerRadius-dwBorderWidth) * sin(fDeltaAngle * (dwIndex-dwCenterIndex) * fPI/180.0) - fCenterY),
                            GetColor(OUTLINE)
                        );
        }
        vaBorder[dwCount*4*2] = vaBorder[0];
        vaBorder[dwCount*4*2+1] = vaBorder[1];
        target.draw(vaBorder, states);
    }

    bool Widget::isFlagSet(WidgetMask mask) const
    {
        return ((uFlag & mask) != 0);
    }

    void Widget::setFlag(WidgetMask mask, bool bStatus)
    {
        if (bStatus)
        {
            uFlag = uFlag | mask;
        }
        else
        {
            uFlag = uFlag & ~mask;
        }
    }
}
