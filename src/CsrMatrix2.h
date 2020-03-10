/**
 * @file   sparseMatrix.h
 * @author Zhu Hanqing
 * @date   Jul 2019
 */
#ifndef _CSRMATRIX_H__
#define _CSRMATRIX_H__

#include <iostream>
#include <vector>
#include "Vector.h"
template <typename TIndex, typename TData>
class CsrMatrix {
    private:
        //statement
        Vector <int, int> _rowOffset;
        Vector <int, int> _columnIndices;
        Vector <int, TData> _values;

        int _numNodes = 0;
        int _numEdges = 0;
        /// @papram _typeId
        /// typeId = 0     Adjacency Matrix 
        /// typeId = 1     Diagonal  Matrix
        int _typeId   = 0;

    public:
        //construct
        explicit CsrMatrix () = default ;
        /// @brief brief describution of below
        /// @param first: nodes num
        /// @param second: edges num
        /// @param third: nodes connect range; nodes connect --> node
        /// @return ->->
        explicit CsrMatrix (int numNodes)
            :_rowOffset(numNodes + 1), _numNodes(numNodes)
        {
            std::cout<<"Call CsrMatrix constructor "<<std::endl;
            std::cout<<"construct successfully"<< std::endl;       
        }
        //copy construct
        CsrMatrix& CsrMatrixCopy (CsrMatrix& other)
        {
            std::cout<<"Call CsrMatrix constructor"<<std::endl;
            int _numNodesCopy;
            int _numEdgesCopy;
            int _typeIdCopy  ;
            int rowOffsetSize, columnIndexSize;
            _numNodesCopy = other.getNodeNum();
            _numEdgesCopy = other.getEdgeNum();
            _typeIdCopy   = other.getTypeId();
            
            // only copy can not do = for return reference not a copy one --> mem leak
            Vector <int, int> _rowOffsetCopy(other.getRowOffsetVec());
            Vector <int, int> _columnIndicesCopy(other.getColumnIndexVec());
            Vector <int, TData> _valuesCopy(other.getValuesVec());
           
            // = 
            _numNodes = _numNodesCopy;
            _numEdges = _numEdgesCopy;
            _typeId   = _typeIdCopy;

            rowOffsetSize = _rowOffsetCopy.size();
            _rowOffset.resize(rowOffsetSize);
            columnIndexSize = _columnIndicesCopy.size();
            _columnIndices.resize(columnIndexSize);
            _values.resize(columnIndexSize);
            for (int i = 0; i < rowOffsetSize; ++i)
            {
                _rowOffset[i] = _rowOffsetCopy[i];

            }
            for (int j = 0; j < columnIndexSize; ++j)
            {
                _columnIndices[j] = _columnIndicesCopy[j];

            }
            for (int k = 0; k < columnIndexSize; ++k)
            {
                _values[k] = _valuesCopy[k];
            }
            /*
            _rowOffsetCopy     = other.getRowOffsetVec();
            _columnIndicesCopy = other.getColumnIndexVec();
            _valuesCopy        = other.getValuesVec();
            */
            std::cout<<"construct successfully"<<std::endl;
        }
        
        void setMatrix(int numNodes, int numEdges, int typeId, Vector<int, int> rowOffset, Vector<int, int>columnIndices, Vector <int, TData> values)
        {
            _numNodes = numNodes;
            _numEdges = numEdges;
            _typeId   = typeId  ;

            _rowOffset     = rowOffset    ;
            _columnIndices = columnIndices;
            _values        = values       ;
        
        }
        //set
        void setMatrixValue(int numNodes, int numEdges,const std::vector<int> &rangeVec,const std::vector<int> &connectVec, const int typeId)
        {   
            if (numNodes != _numNodes)
            {
                std::cout << "matrix size not match "<<std::endl;
            }

            _typeId = typeId;
            _numEdges = numEdges;
            int _numLaplacian = _numNodes + _numEdges * 2;
            
            if (_typeId == 0) //Adjacency 
            {   
                int _numEdgesDoule = _numEdges *2;
                
                _columnIndices.resize(_numEdgesDoule);
                _values.resize(_numEdges*2);
                //data assign
                
                for (int i = 0; i<_numNodes+1;++i){
                    _rowOffset[i] = rangeVec [i];
                }
                    
                for (int i = 0;i<_numEdges*2;++i){
                    _columnIndices[i] = connectVec[i]-1;
                }
                for (int i = 0; i<_numEdges*2; ++i){
                    // for Adjacency all value = 1
                    TData m = 1;
                    _values [i] = m;
                }
            }
            else if (_typeId == 1) //Diagonal
            {   
                _columnIndices.resize(_numNodes);
                _values.resize(_numNodes);
                //data assign
                
                for (int i = 0; i<_numNodes+1;++i){
                    _rowOffset[i] = i;
                }
                std::cout <<std::endl;    
                for (int i = 0;i<_numNodes;++i){
                    _columnIndices[i] = i;
                }
                std::cout <<std::endl;   
                for (int i = 0; i<_numNodes; ++i){
                    // for Diagonal value = edges number
                    _values [i] = rangeVec[i+1] - rangeVec[i];
                }            
            }
            
            else if (_typeId == 2) //laplacian
            {
            
                _columnIndices.resize(_numLaplacian);
                _values.resize(_numLaplacian);

                for (int i = 0; i< _numNodes+1; ++i)
                {
                    _rowOffset[i] = rangeVec[i] + i;
                }
                
                
                _columnIndices [0] = 0;
                _values[0]         = rangeVec[1] - rangeVec[0];
                TData m = -1;
                int cntOffset;
                for (int rowPointer = 0; rowPointer < _numNodes; ++rowPointer)
                {
                    cntOffset = 0;
                    for (int j = 0;j < (rangeVec[rowPointer+1]- rangeVec[rowPointer]);++j)
                    {
                        //get the num of index < rowPointer so the rowPointer's index = rangeVec[rowPointer]+cnt+rowPointer
                        if (rowPointer > connectVec[rangeVec[rowPointer] + j] - 1)
                        {
                            cntOffset +=1 ;
                        }
                        //std::cout<<rowPointer<<" "<<connectVec[rangeVec[rowPointer]+j]<< " "<<cntOffset<<std::endl;

                    }
                    //std::cout<<rowPointer <<" " << cntOffset << std::endl;
                    for (int j = 0;j < (rangeVec[rowPointer+1]- rangeVec[rowPointer]);++j)
                    {
                        if (rowPointer > connectVec[rangeVec[rowPointer] + j] - 1)
                        {
                            _columnIndices[rowPointer+rangeVec[rowPointer]+j] = connectVec[rangeVec[rowPointer] + j] - 1;
                            _values[rowPointer+rangeVec[rowPointer]+j]      = m;
                            //std::cout<<"< "<<_columnIndices[rowPointer+rangeVec[rowPointer]+j]<<" "<<_values[rowPointer+rangeVec[rowPointer]+j]<<std::endl;
                        }
                        else if (rowPointer < connectVec [rangeVec[rowPointer] + j] - 1)
                        {
                            _columnIndices[rowPointer+rangeVec[rowPointer] + j + 1] = connectVec[rangeVec[rowPointer] + j] - 1;
                            _values[rowPointer+rangeVec[rowPointer]+j+1]        = m;
                            //std::cout<<"> "<<_columnIndices[rowPointer+rangeVec[rowPointer]+j+1]<<" "<<_values[rowPointer+rangeVec[rowPointer]+j+1]<<std::endl;
                        }
                        else 
                        {
                            // do nothing
                        }
                        _columnIndices[rangeVec[rowPointer]+cntOffset+rowPointer]  = rowPointer;
                        _values[rangeVec[rowPointer]+cntOffset+rowPointer]         = rangeVec[rowPointer+1] - rangeVec[rowPointer];
                        //std::cout<<"= "<<_columnIndices[rangeVec[rowPointer]+cntOffset+rowPointer]<<" "<<_values[rangeVec[rowPointer]+cntOffset+rowPointer]<<std::endl;
                    }

                }
#if 0
                for (int i = 0; i< _numLaplacian; ++i)
                {
                    std::cout<<_columnIndices[i]<<" ";
                }
                std::cout<<std::endl;
                for (int i = 0; i<_numLaplacian; ++i)
                {
                    std::cout<<_values[i]<<" ";
                }
                std::cout<<std::endl;
#endif
            }

            else 
            {
                std::cout << " no type define" << std::endl;
            }
#if 0
            //print Matrix
            int tempNum;
            bool valueExistFlag = 0;
            for (int i = 0; i < _numNodes; ++i)
            {
                if (_rowOffset[i+1] - _rowOffset[i]!=0)
                {   
                    tempNum = _rowOffset[i+1] - _rowOffset[i];
                    int iii = 0;
                    for (int ii = 0; ii < _numNodes; ++ii)
                    {
                        //std::cout <<"test for out of range" << _rowOffset[i]+iii<<std::endl;
                        // compare column per row 
                        // consider vectot out of range for columnINdex is limited = _numEdgesDoule -1 
                        // but the sort will increase to _numEdgesDoule
                        // need judge wether the sort = _numEdgesDoule befor carry out condition judge
                        // define a flag :  valueExistFlag
                        /*
                        if ((_rowOffset[i]+iii)!=2*_numEdges)
                        {
                            if ( ii == _columnIndices[_rowOffset[i]+iii])
                            {
                                valueExistFlag = 1;
                            }
                            else
                            {
                                valueExistFlag = 0;
                            }
                        }
                        else
                        {
                            valueExistFlag = 0;
                        }
                        */
                        if (ii == _columnIndices[_rowOffset[i]+iii])
                        {
                            std::cout << _values[_rowOffset[i]+iii]<<" ";
                            // use this to solve the out of range problem
                            if (iii < tempNum - 1)
                            {
                                iii+=1;
                            }
                    
                        }
                        else 
                        {
                            std:: cout << 0 << " ";
                        }
                    }
                }
                else 
                {
                    for (int ii = 0; ii<_numNodes; ++ii){
                        std::cout << 0 <<" ";
                    }
                }
                std::cout<< std::endl;                
            }
            
            std::cout<<"finish one matrix print"<<std::endl;
#endif
        }

        //get
        //
        const int& getNodeNum() const
        {
            return _numNodes;
        }
        const int& getEdgeNum() const
        {
            return _numEdges;
        }
        const int& getTypeId() const
        {
            return _typeId;
        }      

        const int& getRowOffset (int i) const             
        {
             return _rowOffset[i];
        }

        const int& getColumnIndex (int i) const
        {
            return _columnIndices[i];
        }
        
        const TData& getValues (int i) const  
        {
            return _values[i];
        }
        
        void setValues (TData& setData, TIndex i)
        {
            _values [i] = setData ;
        }
        
        const Vector <int,int>&    getRowOffsetVec() const     
        { 
            return _rowOffset;     
        }
        const Vector <int,int>&    getColumnIndexVec() const   
        { 
            return _columnIndices; 
        }
        const Vector <int,TData>&  getValuesVec() const        
        { 
            return _values;        
        }
        
        //print Matrix
        void printMatrix()
        {
            //print Matrix
            int tempNum;
            bool valueExistFlag = 0;
            for (int i = 0; i < _numNodes; ++i)
            {
                if (_rowOffset[i+1] - _rowOffset[i]!=0)
                {   
                    tempNum = _rowOffset[i+1] - _rowOffset[i];
                    int iii = 0;
                    for (int ii = 0; ii < _numNodes; ++ii)
                    {
                        //std::cout <<"test for out of range" << _rowOffset[i]+iii<<std::endl;
                        // compare column per row 
                        // consider vectot out of range for columnINdex is limited = _numEdgesDoule -1 
                        // but the sort will increase to _numEdgesDoule
                        // need judge wether the sort = _numEdgesDoule befor carry out condition judge
                        // define a flag :  valueExistFlag
                        /*
                        if ((_rowOffset[i]+iii)!=2*_numEdges)
                        {
                            if ( ii == _columnIndices[_rowOffset[i]+iii])
                            {
                                valueExistFlag = 1;
                            }
                            else
                            {
                                valueExistFlag = 0;
                            }
                        }
                        else
                        {
                            valueExistFlag = 0;
                        }
                        */
                        if (ii == _columnIndices[_rowOffset[i]+iii])
                        {
                            std::cout << _values[_rowOffset[i]+iii]<<" ";
                            // use this to solve the out of range problem
                            if (iii < tempNum - 1)
                            {
                                iii+=1;
                            }
                    
                        }
                        else 
                        {
                            std:: cout << 0 << " ";
                        }
                    }
                }
                else 
                {
                    for (int ii = 0; ii<_numNodes; ++ii){
                        std::cout << 0 <<" ";
                    }
                }
                std::cout<< std::endl;                
            }
            
            std::cout<<"finish one matrix print"<<std::endl;
        }
};

#endif

