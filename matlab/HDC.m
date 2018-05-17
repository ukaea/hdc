%EXAMPLE_INTERFACE Example MATLAB class wrapper to an underlying C++ class
classdef HDC < handle
    properties (SetAccess = private, Hidden = true)
        objectHandle; % Handle to the underlying C++ class instance
    end
    methods
        %% Constructor - Create a new C++ class instance
        function this = HDC(varargin)
            this.objectHandle = hdc_mex('new', varargin{:});
        end

        %% Destructor - Destroy the C++ class instance
        function delete(this)
            % hdc_mex('delete', this.objectHandle);
            disp('do not destroy HDC >:-(')
        end

        function varargout = set_data(this, data)
            inp = {data};
            [varargout{1:nargout}] = hdc_mex('set_data', this.objectHandle, inp{:});
        end

        function varargout = get_data(this)
            inp = {};
            [varargout{1:nargout}] = hdc_mex('get_data', this.objectHandle, inp{:});
        end

        function varargout = dump(this)
            [varargout{1:nargout}] = hdc_mex('dump', this.objectHandle);
        end

    end
end
