#include <iostream>
#include <vector>

#define BLOCK_NUMBER 10
#define BLOCK_SIZE (1 << 10)

class Block
{
    friend class Manager;
private:
    int _size;
    char* _data;
    int _id;

public:
    Block() {}
    Block(int size, char* data, int id) : _size(size), _data(data), _id(id) {}
    int get_size() { return _size; }
    char* get_data() { return _data; }
};

class Manager
{
    char* _data = nullptr;
    std::vector<Block> _blocks;
    int _blocks_size = 0;
    int _blocks_id = 0;

public:
    Manager()
    {
        _blocks = std::vector<Block>(10);
        _blocks.clear();
        _data = new char[BLOCK_NUMBER * BLOCK_SIZE];
        memset(_data, '+', BLOCK_NUMBER * BLOCK_SIZE);
    }

    Block* add_block(int megabyte_size)
    {
        if (megabyte_size + _blocks_size > BLOCK_NUMBER)
            return nullptr;
        _blocks.push_back(Block(megabyte_size, _data + _blocks_size * BLOCK_SIZE, _blocks_id++));
        _blocks_size += megabyte_size;
        return &_blocks.back();
    }

    void delete_block(Block*& block)
    {
        int index = -1;
        for (int i = 0; i < _blocks.size(); i++)
            if (block->_id == _blocks[i]._id)
            {
                index = i;
                break;
            }
        if (index == -1)
            return;

        char* ptr = block->_data;
        _blocks_size -= block->_size;
        _blocks.erase(_blocks.begin() + index); // удаление элемента с индекосм index ( _blocks.Remove([index]) )
        for (int i = index; i < _blocks.size(); i++)
        {
            _blocks[i]._data = ptr;
            ptr += _blocks[i]._size * BLOCK_SIZE;
        }
        block = nullptr;
    }

    Block* unite_blocks(Block*& block1, Block*& block2)
    {
        int new_size = block1->_size + block2->_size;
        std::vector<char> save(new_size * BLOCK_SIZE);
        memcpy(save.data(), block1->_data, block1->_size * BLOCK_SIZE);
        memcpy(save.data() + block1->_size * BLOCK_SIZE, block2->_data, block2->_size * BLOCK_SIZE);
        delete_block(block1);
        delete_block(block2);
        auto block = add_block(new_size);
        memcpy(block->_data, save.data(), new_size * BLOCK_SIZE);
        return block;
    }
};

int main()
{
    Manager mng;
    auto blk = mng.add_block(2);
    memcpy(blk->get_data(), "vazhnaya infa", sizeof("vazhnaya infa"));

    auto blk2 = mng.add_block(7);
    memcpy(blk2->get_data(), "escho infa", sizeof("escho infa"));

    std::cout << blk->get_data() << std::endl;
    std::cout << blk2->get_data() << std::endl;

    mng.delete_block(blk2);
    std::cout << blk->get_data() << std::endl;
    std::cout << blk2 << std::endl;

    auto blk3 = mng.add_block(5);
    memcpy(blk3->get_data(), "krutaya infa", sizeof("krutaya infa"));

    auto blk4 = mng.add_block(4);

    std::cout << blk->get_data() << std::endl;
    std::cout << blk3->get_data() << std::endl;
    std::cout << blk4 << std::endl;

    auto blk5 = mng.unite_blocks(blk, blk3);
    blk5->get_data()[sizeof("vazhnaya infa") - 1] = '\n';
    std::cout << blk5->get_data() << std::endl;
}
