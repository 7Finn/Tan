#include"Block.h"
USING_NS_CC;

Block::Block():b_number(0), isMerging(false), b_lablenumber(NULL), b_layercolor(NULL) {}

Block* Block::createSquareBlock(int number, int width, int height) {
	Block* block = new Block();
	if (block && block->init()) {
		auto physicsBody = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(0.1f, 1.0f, 0.0f));
		block->autorelease();
		block->setPhysicsBody(physicsBody);
		block->setType(Square);
		block->initSquareBlock(number, width, height);
		return block;
	}
	CC_SAFE_DELETE(block);
	return NULL;
}

Block* Block::createCircleBlock(int number, float radius) {
	Block* block = new Block();
	if (block && block->init()) {
		auto physicsBody = PhysicsBody::createCircle(radius, PhysicsMaterial(0.1f, 1.0f, 0.0f));
		block->autorelease();
		block->setPhysicsBody(physicsBody);
		block->setType(Circle);
		block->initCircleBlock(number, radius);
		return block;
	}
	CC_SAFE_DELETE(block);
	return NULL;
}

int Block::getNumber() {
	return b_number;
}


void Block::setNumber(int number) {
	b_number = number;
	b_lablenumber->setString("");
	if (b_number > 0) {
		b_lablenumber->setString(CCString::createWithFormat("%i", b_number)->getCString());
	}
}

void Block::initSquareBlock(int number, int width, int height) {
	b_number = number;
	b_layercolor = CCLayerColor::create(ccc4(random(75, 255), random(75, 255), random(75, 255), 255), width, height);
	b_layercolor->setPosition(ccp(-(width / 2),-(width / 2)));
	this->addChild(b_layercolor);
	b_lablenumber = CCLabelTTF::create("", "arial.ttf", 25);
	b_lablenumber->setColor(ccc3(0, 0, 0));
	b_lablenumber->setPosition(ccp(b_layercolor->getContentSize().width / 2, b_layercolor->getContentSize().height / 2));
	b_layercolor->addChild(b_lablenumber);
	setNumber(number);
}


void Block::initCircleBlock(int number, float radius) {
	b_number = number;
	b_lablenumber = CCLabelTTF::create("", "arial.ttf", 25);
	b_lablenumber->setColor(ccc3(0, 0, 0));
	b_lablenumber->setPosition(this->getPosition());
	this->addChild(b_lablenumber);
	setNumber(number);
}