#ifndef  EGG_COMMON_H_
#define EGG_COMMON_H_

#include "cocos2d.h"
#include "cocos-ext.h"

//==============================================
template <typename Object>
class MyQueue
{
public:
    
    MyQueue(const unsigned int& size_):m_maxSize(size_),m_nowSize(0),m_stackOfTop(-1),m_headIndex(0)
    {
        m_DataArr = new Object[m_maxSize];
    }
    
    ~MyQueue()
    {
        delete[] m_DataArr;
    }
    
    const unsigned int& GetMaxSize() const {return m_maxSize;}
    const unsigned int& GetNowSize() const {return m_nowSize;}
    
    void Resize(const unsigned int& size_)
    {
        Object* temp = m_DataArr;
        m_DataArr = new Object[size_];
        
        int minSize(0);
        m_nowSize > size_?minSize = size_ : minSize = m_nowSize;
        for (unsigned int i = 0; i < minSize; ++i)
            m_DataArr[i] = temp[i];
        
        delete [] temp;
    }
    
    Object Pop_up_return()
    {
        Object& temp = m_DataArr[m_headIndex];
        Pop_up();
        return temp;
    }
    
    void Pop_up()
    {
        if(0 == m_nowSize)
            return;
        ++m_headIndex;
        if(m_headIndex == m_maxSize)
            m_headIndex = 0;
        --m_nowSize;
    }
    
    void Push_back (const Object& obj_)
    {
        if(m_stackOfTop + 1 == m_maxSize)
        {
            m_stackOfTop = -1;
        }
        
        if(m_nowSize != m_maxSize)
            ++m_nowSize;
        
        else if(m_nowSize == m_maxSize)
        {
            ++m_headIndex;
            if(m_headIndex == m_maxSize)
                m_headIndex = 0;
        }
        
        m_DataArr[++m_stackOfTop] = obj_;
    }
    
    Object& operator [] (const unsigned int& index_)
    {
        unsigned int trueIndex(0);
        
        m_headIndex + index_ >= m_maxSize?
        trueIndex = m_headIndex + index_ - m_maxSize:
        trueIndex = m_headIndex + index_;
        return m_DataArr[trueIndex];
    }
    
    Object& LastElement()
    {
        //if(m_nowSize != m_maxSize)
        // return (*this)[m_nowSize - 1];
        //else
        return (*this)[m_nowSize - 1];
    }
    
private:
    Object*         m_DataArr;
    int             m_stackOfTop;
    unsigned int    m_nowSize;
    unsigned int    m_maxSize;
    unsigned int    m_headIndex;
};//==============================================

class GLDrawLayer:public cocos2d::CCLayer
{
public:
    GLDrawLayer();
    ~GLDrawLayer();
    virtual void draw();
    void move(const cocos2d::CCPoint& offset_);
    void setDrawRectStart(const cocos2d::CCPoint& start_);
    void setDrawRectEnd(const cocos2d::CCPoint& end_);
    void setDrawRect(const cocos2d::CCRect& rect_);
    void setDrawPoint(const cocos2d::CCPoint& point_);
    
    const cocos2d::CCPoint& getDrawRectStart() {return m_drawRectStart;}
    const cocos2d::CCPoint& getDrawRectEnd() {return m_drawRectEnd;}
    cocos2d::CCRect getNowRect();
private:
    cocos2d::CCPoint m_drawRectStart;
    cocos2d::CCPoint m_drawRectEnd;
};

//==============================================


class IconListComponent
{
public:
    IconListComponent(const cocos2d::CCPoint& firstIconPos_, const cocos2d::CCSize& iconSize_, const int& MaxShow_ = 8);
    ~IconListComponent();
    
    void goLeftPage();
    void goRightPage();

    void clean();
    
    void addList(cocos2d::CCSprite* spr_, bool bIsDetail = true);
    
    void swapIndex(const int& trueIndex1_, const int& trueIndex2_);
    
    void copyIndex(const int& index_);
    
    void updateIcon(const int& index_);
    
    bool deletIconByShowIndex(const int& showIndex_);
    
    bool deletIconByTrueIndex(const int& trueIndex_);
    
    int getTrueIndexByShowIndex(const int& showIndex_);
    int getShowIndexByTrueIndex(const int& trueIndex_);
    
    const cocos2d::CCPoint& getIconPositionByShowIndex(const int& showIndex_);
    
    cocos2d::CCArray* getShowList(){return m_showList;}
    cocos2d::CCSprite* getMainLayer(){return m_layer;}
    
    int getSize() {return m_trueList->count();}
private:
    void _goPage(const int& page_);
private:
    cocos2d::CCSprite* m_layer;
    cocos2d::CCArray*  m_showList;
    cocos2d::CCArray*  m_trueList;
    int m_nowPage;
    int m_maxPage;
    int m_maxShow;
    cocos2d::CCSize m_iconSize;
    cocos2d::CCPoint m_firstIconPos;
};

//==================================================

namespace cocos2d
{
    class CCMenuItemImageFont:public CCMenuItemImage
    {
    public:
        CCMenuItemImageFont(){}
        virtual ~CCMenuItemImageFont(){}
        void setFont(const char* str_, const int& size_);
        virtual void selected();
        virtual void unselected();
        static CCMenuItemImageFont* create(const char *normalImage, const char *selectedImage,
                                                         CCObject* target, SEL_MenuHandler selector);

        static CCMenuItemImageFont* create();

    protected:
        cocos2d::CCLabelTTF* m_labelTTF;
        cocos2d::CCPoint m_labelPos;
    };
}

//==================================================



#ifdef _WIN32
#define KEY_DOWN(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 1 : 0)  
#define KEY_UP(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 0 : 1)  

std::string G2U(const char* gb2312);
#define ZH2U(zh_) (G2U(zh_).c_str())

std::string W2C(const wchar_t* src_);

#endif

#ifdef  __APPLE__
#define ZH2U(zh_) (zh_)
#endif

#ifdef _WIN32
const char* const SOURCE_ROOT = "Resources/";
#endif

#ifdef __APPLE__
const char* const SOURCE_ROOT = "";
#endif

bool floatEqu(const float& f1, const float& f2);

std::string GetResPath(const char* res_);
#define RES_PATH(rs_) (GetResPath(rs_).c_str())

cocos2d::CCSprite* CreatePureColorSpr(const int& w_, const int& h_, const unsigned int& col_);

cocos2d::CCRect MakeRect(const int& x1, const int& y1, const int& x2, const int& y2);

void ScaleAndMoveCCSprite2Rect(cocos2d::CCSprite* spr_, const cocos2d::CCRect& rect_);

void GetEdge1DPoint(const float& p1, const float& p2, const float& pi, float& outp);
void GetEdge2DPoint(const cocos2d::CCPoint& p1, const cocos2d::CCPoint& p2, const cocos2d::CCPoint& pi, cocos2d::CCPoint& outp);
void SetBoxTextByVar(const int& var_, cocos2d::extension::CCEditBox* box_);

cocos2d::CCMenuItem* AddFontBtn(const char *value_, cocos2d::CCObject* target_, cocos2d::SEL_MenuHandler selector_,
                                                               const cocos2d::CCPoint& pos_, cocos2d::CCMenu* menu_, const int& fontSize_ = 17);

bool CheckSpriteTouch(cocos2d::CCSprite* spr_, const cocos2d::CCPoint& touch_);

cocos2d::CCSprite* CheckSpriteArrTouch(cocos2d::CCArray* sprArray_, const cocos2d::CCPoint& touch_,
                                                                int& outIndex_, bool isNewSpr_ = false);


cocos2d::CCSprite* CreateAndAddMask(cocos2d::CCSprite* ref_, cocos2d::CCLayer* lay_);

void AddSprArrToLayer(cocos2d::CCArray* sprArray_, cocos2d::CCLayer* lay_);
void RemoveSprArrFromLayer(cocos2d::CCArray* sprArray_, cocos2d::CCLayer* lay_);






#endif//endif EGG_COMMON_H_